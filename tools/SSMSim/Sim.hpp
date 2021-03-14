#include <iostream>
#include "Logger.h"
#include <stdint.h>
#include <string>
#include <pthread.h>



class Sim
{
public:
	Sim(
		std::string IP,
		uint16_t    Port);

	int Start(void);

    void Listener(void);

    static void* c_Listener(void *arg);

protected:
	Sim();

private:
	Logger      Log;

	std::string IP;
    uint16_t    Port;

	int         Socket;

	pthread_t   ListenerThread;
};
