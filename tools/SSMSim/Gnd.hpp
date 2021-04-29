#include <iostream>
#include "Logger.h"
#include <stdint.h>
#include <string>
#include <pthread.h>



class Gnd
{
public:
	Gnd(
		std::string tlmIP,
		uint16_t    tlmPort,
		uint16_t    cmdPort);

	int Start(void);

    void Listener(void);
    void SendTlm(const uint8_t *buffer, uint16_t size);

    static void* c_Listener(void *arg);

protected:
	Gnd();

private:
	Logger      Log;

	std::string TlmIP;
    uint16_t    TlmPort;
    uint16_t    CmdPort;

	int         TlmSocket;
	int         CmdSocket;

	pthread_t   ListenerThread;
};
