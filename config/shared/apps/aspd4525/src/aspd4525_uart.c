#include <stdio.h>
#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "cfe.h"
#include "aspd4525_events.h"



/**
 * @brief This is the version of the protocol used to communicate between the arduino code and the linux code
 * 
 */
#define UAH_COMMON_PROTOCOL_VERSION     (2)

/**
 * @brief This is the sync data used to recognize the beginning of a packet
 * 
 */
#define UAH_COMMON_PILOT_DATA           (0xdeadbeef)

#if defined (__AVR__) || (__avr__)
/**
 * @brief This is the packet struct used on the arduino side
 * 
 */
typedef struct{
  unsigned long syncCode;
  unsigned char protocolVersion;
  unsigned int packetSize;
  unsigned int pressureReading;
  unsigned int temperatureReading;
  unsigned char status;
  unsigned char checkSum;
} uah_packer_tab_t;
#else
/**
 * @brief The pragma is needed to pack as bytes
 * 
 */
#pragma pack(push, 1)
/**
 * @brief This is the packet struct used on the linux side
 * 
 */
typedef struct{
  unsigned int syncCode;
  unsigned char protocolVersion;
  unsigned short packetSize;
  unsigned short pressureReading;
  unsigned short temperatureReading;
  unsigned char status;
  unsigned char checkSum;
} uah_packer_tab_t;
#pragma pack(pop)
#endif

#define UAH_APP_UART_PORTNAME       "/dev/ttyACM0"

enum strategy { SEARCH_SYNC_START, SEARCH_SYNC_END, VERIFY_PROTOCOL_VERSION, VERIFY_CHECKSUM };

typedef struct{
        int fd, status;
        enum strategy uah_state;
        unsigned char buf[20];
        unsigned char collectionBuf[sizeof(uah_packer_tab_t)];
        int collectionBufIdx;
        unsigned char checksum;
        uah_packer_tab_t* cookieCutterStructP;
        unsigned char syncByte;
} uah_app_t;


/**
 * @brief This function sets up the UART attributes
 * 
 * @param fd UART Port File Descritor Handle
 * @param speed Speed
 * @param parity Parity CHeck
 * @return boolean TRUE if successful, FALSE if not.
 */
boolean UAH_setInterfaceAttribs (int fd, int speed, int parity);

/**
 * @brief This sets up blocking functionality for the UART
 * 
 * @param fd UART Port File Descritor Handle
 * @param should_block 1 for yes, 0 for no.
 * @return boolean TRUE if successful, FALSE if not.
 */
boolean  UAH_setBlocking (int fd, int should_block);

/**
 * @brief This function sets up the UART Pipe given a portname
 * 
 * @param fd UART Port File Descritor Handle
 * @param portname path to the USB port name
 * @return boolean TRUE if successful, FALSE if not.
 */
boolean UAH_pipeSetup(int* fd, const char* portname);

/**
 * @brief This is a debug control macro.
 * Higher the number, more verbose the prontf statements.
 * 
 */
#define DEBUG_LEVEL  (5)

uah_app_t uah_app;

boolean UAH_setInterfaceAttribs (int fd, int speed, int parity)
{
    struct termios tty;
    if (tcgetattr (fd, &tty) != 0)
    {
        fprintf (stderr, "error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        fprintf (stderr, "error %d from tcsetattr", errno);
        return FALSE;
    }
    return TRUE;
}

boolean UAH_setBlocking (int fd, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        fprintf (stderr, "error %d from tggetattr", errno);
        return FALSE;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        fprintf (stderr, "error %d setting term attributes", errno);
        return FALSE;
    }
    return TRUE;
}

boolean UAH_pipeSetup(int* fd, const char* portname)
{
    boolean status;
    *fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (*fd < 0)
    {
        fprintf (stderr, "error %d opening %s: %s", errno, portname, strerror (errno));
        return FALSE;
    }

    status = UAH_setInterfaceAttribs (*fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    if (FALSE == status) {
        return FALSE;
    }
    status = UAH_setBlocking (*fd, 0);                // set no blocking
    if (FALSE == status) {
        return FALSE;
    }
    return TRUE;
}

boolean ASPD4525_Custom_Measure(int16 *pressureDiffP, int16 *temperatureP, uint8 *statusP)
{
    boolean returnBool = TRUE;
    if ((pressureDiffP==NULL)||(temperatureP==NULL)||(statusP==NULL)) {
        returnBool = FALSE;
        (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "ASPD4525 Custom_Measure Null Pointer");
        goto end_of_function;
    }
    int n = read(uah_app.fd, uah_app.buf, 1);
    usleep(100*4);
    for (int j =0; j<n; j++) {
        #if (DEBUG_LEVEL>8)
        printf("%d, 0x%02X\n", buf[j], buf[j]);
        #endif /*(DEBUG_LEVEL>8)*/

        switch (uah_app.uah_state) {
            case SEARCH_SYNC_START:
                if (uah_app.syncByte==uah_app.buf[j]) {
                    uah_app.collectionBuf[uah_app.collectionBufIdx] = uah_app.buf[j];
                    uah_app.checksum+=uah_app.collectionBuf[uah_app.collectionBufIdx];
                    uah_app.collectionBufIdx++;
                    uah_app.uah_state = SEARCH_SYNC_END;
                    
                    #if (DEBUG_LEVEL>5)
                    printf("uah_state = SEARCH_SYNC_END\n");
                    #endif
                }
                break;
            case SEARCH_SYNC_END:
                if (uah_app.collectionBufIdx<sizeof(UAH_COMMON_PILOT_DATA)) {
                    uah_app.collectionBuf[uah_app.collectionBufIdx] = uah_app.buf[j];
                    uah_app.checksum+=uah_app.collectionBuf[uah_app.collectionBufIdx];
                    uah_app.collectionBufIdx++;
                }
                if (sizeof(UAH_COMMON_PILOT_DATA) == uah_app.collectionBufIdx) {
                    if (UAH_COMMON_PILOT_DATA == uah_app.cookieCutterStructP->syncCode) {
                        uah_app.uah_state = VERIFY_PROTOCOL_VERSION;

                        #if (DEBUG_LEVEL>5)
                        printf("uah_state = VERIFY_PROTOCOL_VERSION\n");
                        #endif

                    } else {
                        uah_app.collectionBufIdx =0;
                        uah_app.checksum=0;
                        uah_app.uah_state = SEARCH_SYNC_START;

                        #if (DEBUG_LEVEL>5)
                        printf("uah_state = SEARCH_SYNC_START\n");
                        #endif

                    }
                }
                break;
            case VERIFY_PROTOCOL_VERSION:
                uah_app.collectionBuf[uah_app.collectionBufIdx] = uah_app.buf[j];
                uah_app.checksum+=uah_app.collectionBuf[uah_app.collectionBufIdx];
                uah_app.collectionBufIdx++;
                if (UAH_COMMON_PROTOCOL_VERSION == uah_app.cookieCutterStructP->protocolVersion) {
                    uah_app.uah_state = VERIFY_CHECKSUM;

                    #if (DEBUG_LEVEL>5)
                    printf("uah_state = VERIFY_CHECKSUM\n");
                    #endif

                } else {
                    uah_app.collectionBufIdx =0;
                    uah_app.checksum=0;
                    uah_app.uah_state = SEARCH_SYNC_START;

                    #if (DEBUG_LEVEL>5)
                    printf("uah_state = SEARCH_SYNC_START\n");
                    #endif
                }
                break;
            case VERIFY_CHECKSUM:
                if (uah_app.collectionBufIdx<sizeof(uah_packer_tab_t)) {
                        uah_app.collectionBuf[uah_app.collectionBufIdx] = uah_app.buf[j];
                }
                #if (DEBUG_LEVEL>7)
                printf("idx=%d\tsize=%d\tchecksum=%d\n", uah_app.collectionBufIdx, (int)sizeof(uah_packer_tab_t), checksum);
                #endif
                if (sizeof(uah_packer_tab_t)==(uah_app.collectionBufIdx+1)) {
                    if (uah_app.checksum == uah_app.cookieCutterStructP->checkSum) {
                        #if (DEBUG_LEVEL>3)
                        printf("Pressure = %d\n", uah_app.cookieCutterStructP->pressureReading);
                        printf("Temperature = %d\n", uah_app.cookieCutterStructP->temperatureReading);
                        printf("Status = %d\n", uah_app.cookieCutterStructP->status);
                        #endif
                        *pressureDiffP =       uah_app.cookieCutterStructP->pressureReading;
                        *temperatureP =    uah_app.cookieCutterStructP->temperatureReading;
                        *statusP =              uah_app.cookieCutterStructP->status;
                    } else {
                        uah_app.collectionBufIdx = 0;
                        uah_app.checksum=0;
                        uah_app.uah_state = SEARCH_SYNC_START;

                        #if (DEBUG_LEVEL>5)
                        printf("uah_state = SEARCH_SYNC_START\n");
                        #endif

                    }
                } else {
                    uah_app.checksum+=uah_app.collectionBuf[uah_app.collectionBufIdx];
                    uah_app.collectionBufIdx++;
                }
                break;
            default:
                #if (DEBUG_LEVEL>5)
                printf("uah_state = %d\n", uah_app.uah_state);
                #endif
                break;
        }
    }
end_of_function:
    return returnBool;
}

boolean ASPD4525_Custom_Init(void) {
    uah_app.status = UAH_pipeSetup(&(uah_app.fd), UAH_APP_UART_PORTNAME);
    if (TRUE==uah_app.status) {
        uah_app.uah_state = SEARCH_SYNC_START;
        uah_app.collectionBufIdx =0;
        uah_app.checksum=0;
        uah_app.cookieCutterStructP = (uah_packer_tab_t*) (uah_app.collectionBuf);
        uah_app.syncByte = UAH_COMMON_PILOT_DATA & 0xff;
        
        #if (DEBUG_LEVEL>5)
        printf("packetSize = %d\n", (int)sizeof(uah_packer_tab_t));
        printf("uah_state = SEARCH_SYNC_START\n");
        #endif
        return uah_app.status;
    } else {
        return FALSE;
    }
}

void ASPD4525_Custom_InitData(void)
{
    CFE_PSP_MemSet(&uah_app, 0, sizeof(uah_app));
    return;
}