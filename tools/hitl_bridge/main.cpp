#include <argp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include "mavlink.h"
#include "uio_mailbox.h"
#include "sed_msgs.h"


#define SIM_DEFAULT_PORT              (4560)
#define SIM_MAX_MESSAGE_SIZE          (1500)
#define SED_DEFAULT_MAILBOX_UIO_PATH  "/dev/uio1"
#define SED_MAILBOX_SIZE              (0x10000)
#define SED_MAILBOX_BLOCKING_DELAY    (1)


#define SED_BARO_1_TLM_MSG_ID    (0x0802)
#define SED_BARO_2_TLM_MSG_ID    (0x0803)
#define SED_IMU_1_TLM_MSG_ID     (0x0804)
#define SED_IMU_2_TLM_MSG_ID     (0x0805)
#define SED_MAG_1_TLM_MSG_ID     (0x0806)
#define SED_MAG_2_TLM_MSG_ID     (0x0807)
#define SED_PWM_TLM_MSG_ID       (0x0808)

#define SED_MSG_CADU             (0x01020304)

#define ACC_X_SCALE              (1.0f)
#define ACC_Y_SCALE              (1.0f)
#define ACC_Z_SCALE              (1.0f)
#define ACC_X_OFFSET             (0.0f)
#define ACC_Y_OFFSET             (0.0f)
#define ACC_Z_OFFSET             (0.0f)
#define ACC_UNIT                 (9.80665f)
#define ACC_DIVIDER              (2048)
#define GYRO_X_SCALE             (1.0f)
#define GYRO_Y_SCALE             (1.0f)
#define GYRO_Z_SCALE             (1.0f)
#define GYRO_X_OFFSET            (0.0f)
#define GYRO_Y_OFFSET            (0.0f)
#define GYRO_Z_OFFSET            (0.0f)
#define GYRO_UNIT                (0.0174532f)
#define GYRO_DIVIDER             (16.4f)
#define MAG_X_SCALE              (1.0f)
#define MAG_Y_SCALE              (1.0f)
#define MAG_Z_SCALE              (1.0f)
#define MAG_X_OFFSET             (0.0f)
#define MAG_Y_OFFSET             (0.0f)
#define MAG_Z_OFFSET             (0.0f)
#define MAG_UNIT                 (1.0f)
#define MAG_DIVIDER              (820.0f)

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

const char *argp_program_version = "HITL Bridge 1.0";
const char *argp_program_bug_address = "<mbenson@windhoverlabs.com>";


/* Program documentation. */
static char doc[] = "Hardware in the Loop (HITL) Bridge feeds sensor data from "
		"the simulation to the Sensor Effector Domain (SED), and actuator "
		"commands from SED to the simulation.";

/* A description of the arguments we accept. */
static char args_doc[] = "--address <ADDRESS> [--port <PORT> --device <DEVICE>]";

/* The options we understand. */
static struct argp_option options[] =
{
    { "address", 'a', "ADDRESS", 0, "IP address of the simulation." },
    { "port", 'p', "PORT", OPTION_ARG_OPTIONAL, "Simulation TCP port."},
    { "device", 'd', "DEVICE", OPTION_ARG_OPTIONAL, "SED UIO device path."},
    { 0 } };

/* Used by main to communicate with parse_opt. */
typedef struct
{
	char       *address;
	bool        address_set;
	uint32_t    port;
	bool        port_set;
	const char *device;
	bool        device_set;
} arguments_t;




typedef enum
{
	MPS_WAITING_FOR_CADU,
	MPS_WAITING_FOR_SIZE,
	MPS_PARSING_MESSAGE,
	MPS_WAITING_FOR_CHECKSUM,
	MPS_MESSAGE_COMPLETE,
	MPS_BUFFER_OVERFLOW
} Mailbox_Parser_State_t;


typedef struct
{
	Mailbox_Parser_State_t State;
	unsigned int           CurrentChecksum;
	unsigned int           InputBufferCursor;
	unsigned int           FullMessageSize;
	unsigned int           Size;
} Mailbox_Parser_Handle_t;



/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	 know is a pointer to our arguments structure. */
	arguments_t *arguments = (arguments_t *) state->input;

	switch (key)
	{
		case 'a':
		{
			arguments->address = arg;
			arguments->address_set = true;
			break;
		}

		case 'p':
		{
			if(arg != 0)
			{
			    arguments->port = atoi(arg);
			    arguments->port_set = true;
			}
			break;
		}

		case 'd':
		{
			if(arg != 0)
			{
				arguments->device = arg;
			    arguments->device_set = true;
			}
			break;
		}

		case ARGP_KEY_ARG:
		{
			break;
		}

		case ARGP_KEY_END:
		{
			/* Verify arguments, starting with the address file. */
			if (arguments->address_set == false)
			{
				printf("Error:  address not set.\n");
				argp_usage(state);
				return ARGP_KEY_ERROR;
			}

			/* Set default port if not set. */
			if (arguments->port_set == false)
			{
				arguments->port = SIM_DEFAULT_PORT;
				arguments->port_set = true;
			}

			/* Set default device path if not set. */
			if (arguments->device_set == false)
			{
				arguments->device = SED_DEFAULT_MAILBOX_UIO_PATH;
				arguments->device_set = true;
			}

			break;
		}

		default:
		{
			return ARGP_ERR_UNKNOWN;
		}
	}

	return 0;
}

/* Our argp parser. */
static struct argp argp =
{ options, parse_opt, args_doc, doc };


enum class SensorSource {
  ACCEL         = 0b111,
  GYRO          = 0b111000,
  MAG           = 0b111000000,
  BARO          = 0b1101000000000,
  DIFF_PRESS    = 0b10000000000,
};

void    *SimListener(void* data);
void    *SedListener(void* data);
void    *SedSender(void* data);
int32_t  SendHeartbeat(void);

bool     IMU_Apply_Platform_Rotation(float *X, float *Y, float *Z);
bool     Mag_Apply_Platform_Rotation(float *X, float *Y, float *Z);
unsigned int ParseMessage(Mailbox_Parser_Handle_t *Handle, unsigned int Input, unsigned int* Buffer, unsigned int *BufferSize);


int sock;
void *mailbox = 0;
TlmMsg_t TlmMsg __attribute__ ((aligned(4)));
IMU_Measurement_t  msgImu;
MAG_Message_t      msgMag;
BARO_Message_t     msgBaro;
float              globalTemperature;
bool               msgImuReady = false;
bool               msgMagReady = false;
bool               msgBaroReady = false;
bool               accValid = false;
bool               gyroValid = false;
bool               temperatureValid = false;
bool               pressureValid = false;
Mailbox_Parser_Handle_t Parser;



int main(int argc, char **argv)
{
	arguments_t arguments;
    int         rc;
    pthread_t   simThreadID;
    pthread_t   sedListenerThreadID;
    pthread_t   sedSenderThreadID;
	error_t     parse_error;

	memset(&arguments, 0, sizeof(arguments));
	memset(&msgImu, 0, sizeof(msgImu));

	/* Parse our arguments; every option seen by parse_opt will
	 be reflected in arguments. */
	parse_error = argp_parse(&argp, argc, argv, 0, 0, &arguments);
	if (parse_error == 0)
	{
		printf("Starting servers.\n");

	    rc = pthread_create(&simThreadID, 0, SimListener, (void*)&arguments);
	    if(rc)			/* could not create thread */
	    {
	        printf("ERROR: Failed to create SimListener thread (%d). Terminating.\n", rc);
	        exit(-1);
	    }

	    rc = pthread_create(&sedListenerThreadID, 0, SedListener, (void*)&arguments);
	    if(rc)			/* could not create thread */
	    {
	        printf("ERROR: Failed to create SedListener thread (%d). Terminating.\n", rc);
	        exit(-1);
	    }

	    rc = pthread_create(&sedSenderThreadID, 0, SedSender, (void*)&arguments);
	    if(rc)			/* could not create thread */
	    {
	        printf("ERROR: Failed to create SedSender thread (%d). Terminating.\n", rc);
	        exit(-1);
	    }

		while(1)
		{
			sleep(1);
		}
	}
}



void* SimListener(void* data)
{
	struct             sockaddr_in serv_addr;
	arguments_t       *arguments = (arguments_t*)data;
    int                rc;
    int                reuseaddr = 1;
	char               buffer[SIM_MAX_MESSAGE_SIZE] = {};
	int32_t            size = SIM_MAX_MESSAGE_SIZE;
	struct hostent    *he;

	memset(&msgImu, 0, sizeof(msgImu));

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		printf("ERROR: Could not create socket. Terminating.\n");
		exit(-1);
	}

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&buffer, 0, sizeof(buffer));

    he = gethostbyname(arguments->address);
    if(he == NULL)
    {  /* get the host info */
		printf("ERROR: SIM gethostbyname failed. Terminating.\n");
        exit(-1);
    }

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(arguments->port);
    serv_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(serv_addr.sin_zero), 8);

	rc = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(rc < 0)
    {
		printf("ERROR: SIM connect failed. Terminating.\n");
		exit(-1);
    }

	printf("SIM server started.\n");

	while(1)
	{
		size = read(sock, (char *)buffer, (size_t)size);

		if(size <= 0)
		{
			sleep(1);
		}
		else
		{
			mavlink_message_t msg;
			mavlink_status_t status;
			int32_t i = 0;

			for (i = 0; i < size; ++i)
			{
				if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status))
				{
					switch(msg.msgid)
					{
						case MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW:
						{
							printf("MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW\n");
							mavlink_hil_rc_inputs_raw_t 		decodedMsg;
							mavlink_msg_hil_rc_inputs_raw_decode(&msg, &decodedMsg);
							break;
						}

						case MAVLINK_MSG_ID_HIL_SENSOR:
						{
							mavlink_hil_sensor_t 				decodedMsg;
							mavlink_msg_hil_sensor_decode(&msg, &decodedMsg);

							if(decodedMsg.fields_updated & (uint32_t)SensorSource::ACCEL)
							{
							    /* Apply inverse rotation */
								IMU_Apply_Platform_Rotation(&decodedMsg.xacc, &decodedMsg.yacc, &decodedMsg.zacc);

							    msgImu.AX = ((decodedMsg.xacc / ACC_X_SCALE) + ACC_X_OFFSET) / (ACC_UNIT / ACC_DIVIDER);
							    msgImu.AY = ((decodedMsg.yacc / ACC_Y_SCALE) + ACC_Y_OFFSET) / (ACC_UNIT / ACC_DIVIDER);
							    msgImu.AZ = ((decodedMsg.zacc / ACC_Z_SCALE) + ACC_Z_OFFSET) / (ACC_UNIT / ACC_DIVIDER);

								accValid = true;
							}

							if(decodedMsg.fields_updated & (uint32_t)SensorSource::GYRO)
							{
							    /* Apply inverse rotation */
								IMU_Apply_Platform_Rotation(&decodedMsg.xgyro, &decodedMsg.ygyro, &decodedMsg.zgyro);

							    msgImu.GX = ((decodedMsg.xgyro / GYRO_X_SCALE) + GYRO_X_OFFSET) / (GYRO_UNIT / GYRO_DIVIDER);
							    msgImu.GY = ((decodedMsg.ygyro / GYRO_Y_SCALE) + GYRO_Y_OFFSET) / (GYRO_UNIT / GYRO_DIVIDER);
							    msgImu.GZ = ((decodedMsg.zgyro / GYRO_Z_SCALE) + GYRO_Z_OFFSET) / (GYRO_UNIT / GYRO_DIVIDER);

								gyroValid = true;
							}

							if(decodedMsg.fields_updated & (uint32_t)SensorSource::MAG)
							{
							    /* Apply inverse rotation */
								Mag_Apply_Platform_Rotation(&decodedMsg.xmag, &decodedMsg.ymag, &decodedMsg.zmag);

							    msgMag.RawX = ((decodedMsg.xmag / MAG_X_SCALE) + MAG_X_OFFSET) / (MAG_UNIT / MAG_DIVIDER);
							    msgMag.RawY = ((decodedMsg.ymag / MAG_Y_SCALE) + MAG_Y_OFFSET) / (MAG_UNIT / MAG_DIVIDER);
							    msgMag.RawZ = ((decodedMsg.zmag / MAG_Z_SCALE) + MAG_Z_OFFSET) / (MAG_UNIT / MAG_DIVIDER);

							    msgMagReady = true;
							}

							if(decodedMsg.fields_updated & 0x00000800)
							{
								msgBaro.Temperature = FLOAT_TO_INT(decodedMsg.temperature * 100.0f);

								temperatureValid = true;
							}

							if(decodedMsg.fields_updated & 0x00000400)
							{
								/* decodedMsg.abs_pressure
								 * <<MAYBE>> SIMLIB_SetPressure(decodedMsg.abs_pressure, decodedMsg.diff_pressure);
								 */
							}

							if(decodedMsg.fields_updated & (uint32_t)SensorSource::BARO)
                            {
								msgBaro.Pressure = FLOAT_TO_INT(decodedMsg.pressure_alt * 100.0f);

								pressureValid = true;

				                break;
						    }

							if(gyroValid && accValid)
							{
							    msgImuReady = true;
							}

							if(temperatureValid && pressureValid)
							{
							    msgBaroReady = true;
							}

							break;
						}

						case MAVLINK_MSG_ID_HIL_GPS:
						{
							mavlink_hil_gps_t 					decodedMsg;
							mavlink_msg_hil_gps_decode(&msg, &decodedMsg);
							break;
						}

						case MAVLINK_MSG_ID_HIL_OPTICAL_FLOW:
						{
							mavlink_hil_optical_flow_t 			decodedMsg;
							mavlink_msg_hil_optical_flow_decode(&msg, &decodedMsg);
							break;
						}

						case MAVLINK_MSG_ID_HIL_STATE_QUATERNION:
						{
							mavlink_hil_state_quaternion_t 		decodedMsg;
							mavlink_msg_hil_state_quaternion_decode(&msg, &decodedMsg);
							break;
						}

						case MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE:
						{
							mavlink_vision_position_estimate_t 	decodedMsg;
							mavlink_msg_vision_position_estimate_decode(&msg, &decodedMsg);
							break;
						}

						case MAVLINK_MSG_ID_DISTANCE_SENSOR:
						{
							mavlink_distance_sensor_t 	decodedMsg;
							mavlink_msg_distance_sensor_decode(&msg, &decodedMsg);

							break;
						}

						case MAVLINK_MSG_ID_HEARTBEAT:
						{
							SendHeartbeat();
							break;
						}

						case MAVLINK_MSG_ID_SYSTEM_TIME:
						{
							break;
						}

						case MAVLINK_MSG_ID_SET_MODE:
						{
							break;
						}

						default:
						{
							printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
							break;
						}
					}
				}
			}
		}
	}
}



void* SedListener(void* data)
{
    arguments_t *arguments = (arguments_t*)data;

    mailbox = UIOMB_InitDevice(arguments->device, SED_MAILBOX_SIZE, SED_MAILBOX_BLOCKING_DELAY);
    if(0 == mailbox)
    {
    	printf("Failed to open SED mailbox. Terminating.\n");
    	exit(-1);
    }

	printf("SED server started.\n");

	while(1)
	{
    	int bytesRead;
    	unsigned int buffer[250];
    	char message[1000];
    	Parser.Size = sizeof(buffer);

    	usleep(4000);
		//if(UIOMB_UIO_WaitForInterrupt(2000))
		//{
			bytesRead = UIOMB_MB_Read(mailbox, (unsigned int*)buffer, sizeof(buffer));

	        for(uint32_t i = 0; i < bytesRead; ++i)
	        {
	        	unsigned int State = 0;

	        	State = ParseMessage(&Parser, buffer[i], (unsigned int*)message, &Parser.Size);
	        	if(State == MPS_MESSAGE_COMPLETE)
	        	{
	        		CFE_SB_MsgId_t msgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)message);

	            	switch(msgID)
	            	{
	            	    case SED_PWM_TLM_MSG_ID:
	            	    {
	            	    	PWM_Message_t *msg;
	            	    	mavlink_message_t mavlinkMessage = {};
	            	    	uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
	            	    	mavlink_hil_actuator_controls_t actuatorControlsMsg = {};
	            	    	uint32_t length = 0;

	            	    	msg = (PWM_Message_t*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);

	            	    	for(uint32_t i=0; i < PWM_MAX_OUTPUTS; ++i)
	            	    	{
	            	    		actuatorControlsMsg.controls[i] = msg->Cmd[i];
	            	    	}

	        	    		actuatorControlsMsg.time_usec = 0;
	        	    		actuatorControlsMsg.flags = 0;
	        	    		actuatorControlsMsg.mode = 129;

	        	    		mavlink_msg_hil_actuator_controls_encode(1, 1, &mavlinkMessage, &actuatorControlsMsg);
	        	    		length = mavlink_msg_to_send_buffer(buffer, &mavlinkMessage);

	        	    		send(sock, (char *)buffer, length, 0);

	            	    	break;
	            	    }

	            	    default:
	            	    {
	            	    	printf("ERROR: Received unknown message ID (0x%04x)\n", msgID);
	            	    }
	            	}

	        		Parser.Size = sizeof(buffer);
	        	}
	        }
		//}
	}

	return 0;
}



int32_t SendHeartbeat(void)
{
	int32_t  iStatus = 0;
	mavlink_message_t msg = {};
	uint8_t  buffer[MAVLINK_MAX_PACKET_LEN];
	mavlink_heartbeat_t heartbeatMsg = {};
	uint32_t  length = 0;

	heartbeatMsg.type = MAV_TYPE_GENERIC;
	heartbeatMsg.autopilot = MAV_AUTOPILOT_GENERIC;
	heartbeatMsg.base_mode = 0; //MAV_MODE_FLAG_DECODE_POSITION_SAFETY + MAV_MODE_FLAG_DECODE_POSITION_CUSTOM_MODE;
	heartbeatMsg.custom_mode = 0;
	heartbeatMsg.system_status = MAV_STATE_ACTIVE;
	heartbeatMsg.mavlink_version = 1;

	mavlink_msg_heartbeat_encode(1, 1, &msg, &heartbeatMsg);
	length = mavlink_msg_to_send_buffer(buffer, &msg);

	if(sock != 0)
	{
		send(sock, (char *)buffer, length, 0);
	}

	iStatus = 0;

    return iStatus;
}


void SED_SendMsg(uint16_t MsgID, char *Buffer, uint32_t Size)
{
	uint32_t checksum = 0;
	uint32_t msgSize = (Size + sizeof(CCSDS_TlmPkt_t) + 3)/4;
	uint32_t copyBuffer[msgSize];
	uint32_t CADU = SED_MSG_CADU;

	CCSDS_InitPkt((CCSDS_PriHdr_t*)&TlmMsg, MsgID, Size+sizeof(CCSDS_TlmPkt_t), true);
    memcpy(&TlmMsg.Payload, Buffer, Size);
    memcpy(&copyBuffer[0], &TlmMsg, Size+sizeof(CCSDS_TlmPkt_t));

    UIOMB_MB_Write(mailbox, (unsigned int*)&CADU, 4, SED_MAILBOX_BLOCKING_DELAY);
    UIOMB_MB_Write(mailbox, (unsigned int*)&msgSize, 4, SED_MAILBOX_BLOCKING_DELAY);

    for(uint32_t i = 0; i < msgSize; ++i)
    {
    	checksum += copyBuffer[i];
        UIOMB_MB_Write(mailbox, (unsigned int*)&copyBuffer[i], 4, SED_MAILBOX_BLOCKING_DELAY);
    }
    UIOMB_MB_Write(mailbox, (unsigned int*)&checksum, 4, SED_MAILBOX_BLOCKING_DELAY);
}



void* SedSender(void* data)
{
    arguments_t *arguments = (arguments_t*)data;

	while(1)
	{
		usleep(1000000/250);
		if(mailbox != 0)
		{
			if(msgImuReady)
			{
			    SED_SendMsg(SED_IMU_1_TLM_MSG_ID, (char*)&msgImu, sizeof(msgImu));
			    SED_SendMsg(SED_IMU_2_TLM_MSG_ID, (char*)&msgImu, sizeof(msgImu));
			}

			if(msgMagReady)
			{
			    SED_SendMsg(SED_MAG_1_TLM_MSG_ID, (char*)&msgMag, sizeof(msgMag));
			    SED_SendMsg(SED_MAG_2_TLM_MSG_ID, (char*)&msgMag, sizeof(msgMag));
			}

			if(msgBaroReady)
			{
			    SED_SendMsg(SED_BARO_1_TLM_MSG_ID, (char*)&msgBaro, sizeof(msgBaro));
			    SED_SendMsg(SED_BARO_2_TLM_MSG_ID, (char*)&msgBaro, sizeof(msgBaro));
			}
		}
	}
}



bool IMU_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
	bool returnBool = true;
    float temp;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        returnBool = false;
        goto end_of_function;
    }
    /* TODO move to a table */
    /* ROTATION_ROLL_180_YAW_90 */
    temp = *X;
    *X = *Y;
    *Y = temp;
    *Z = -*Z;

end_of_function:

    return returnBool;
}



bool Mag_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
	bool returnBool = true;
    float temp;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        returnBool = false;
        goto end_of_function;
    }
    /* TODO move to a table */
    /* ROTATION_ROLL_180_YAW_90 */
    temp = *X;
    *X = -*Y;
    *Y = temp;

end_of_function:

    return returnBool;
}




unsigned int ParseMessage(Mailbox_Parser_Handle_t *Handle, unsigned int Input, unsigned int* Buffer, unsigned int *BufferSize)
{
	unsigned int sizeInWords = (*BufferSize+3)/4;

    switch(Handle->State)
	{
        /* Fallthru */
        case MPS_MESSAGE_COMPLETE:
	    case MPS_WAITING_FOR_CADU:
	    {
	    	if(Input == SED_MSG_CADU)
	    	{
		    	Handle->InputBufferCursor = 0;
		    	Handle->CurrentChecksum = 0;
	    		Handle->State = MPS_WAITING_FOR_SIZE;
	    	}
	    	else
	    	{
		    	Handle->State = MPS_WAITING_FOR_CADU;
	    	}
	    	break;
	    }

	    case MPS_WAITING_FOR_SIZE:
	    {
	    	Handle->FullMessageSize = Input;
	    	Handle->State = MPS_PARSING_MESSAGE;
	    	break;
	    }

	    case MPS_PARSING_MESSAGE:
	    {
	    	if(Handle->InputBufferCursor < sizeInWords)
	    	{
	    		Buffer[Handle->InputBufferCursor] = Input;
	    	}
	    	Handle->CurrentChecksum += Input;
	    	Handle->InputBufferCursor++;
	    	if(Handle->InputBufferCursor >= Handle->FullMessageSize)
	    	{
		    	Handle->State = MPS_WAITING_FOR_CHECKSUM;
	    	}
	    	break;
	    }

	    case MPS_WAITING_FOR_CHECKSUM:
	    {
	    	if(Input != Handle->CurrentChecksum)
	    	{
	    		/* Checkum mismatch. */
		    	Handle->State = MPS_WAITING_FOR_CADU;
	    	}
	    	else
	    	{
	    		*BufferSize = Handle->InputBufferCursor*4;
		    	Handle->State = MPS_MESSAGE_COMPLETE;
	    	}
	    	break;
	    }
	}

    return Handle->State;
}
