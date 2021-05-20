#include <stdint.h>
#include "ccsds.h"


#define TLM_PAYLOAD_SIZE_MAX      (1000)
#define PWM_MAX_OUTPUTS           (16)


typedef struct
{
	CCSDS_TlmPkt_t Header;
	uint8_t        Payload[TLM_PAYLOAD_SIZE_MAX];
} TlmMsg_t;

typedef struct
{
    uint16_t AX;
    uint16_t AY;
    uint16_t AZ;
    uint16_t GX;
    uint16_t GY;
    uint16_t GZ;
} IMU_Measurement_t;


typedef struct
{
	int16_t RawX;
	int16_t RawY;
	int16_t RawZ;
} MAG_Message_t;


typedef struct
{
	int32_t Pressure;
	int32_t Temperature;
} BARO_Message_t;


typedef struct
{
    uint16_t Cmd[PWM_MAX_OUTPUTS];
} PWM_Message_t;


