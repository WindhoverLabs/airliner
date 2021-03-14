#include <iostream>
#include "Logger.h"
#include <stdint.h>
#include <string>
#include <pthread.h>



class OBC
{
public:
	OBC(std::string name,
		uint16_t    fswTlmPort,
		std::string fswCmdIP,
		uint16_t    fswCmdPort,
		std::string gndTlmIP,
		uint16_t    gndTlmPort,
		uint16_t    gndCmdPort,
		std::string simIP,
		uint16_t    simPort);

	int Start(void);

    void FswTlmListener(void);
    void GndCmdListener(void);
    void SimListener(void);

    static void* c_FswTlmListener(void *arg);
    static void* c_GndCmdListener(void *arg);
    static void* c_SimListener(void *arg);

protected:
	OBC();

private:
	Logger      Log;
	std::string Name;

    uint16_t    FswTlmPort;
	std::string FswCmdIP;
    uint16_t    FswCmdPort;
	std::string GndTlmIP;
    uint16_t    GndTlmPort;
    uint16_t    GndCmdPort;
	std::string SimIP;
    uint16_t    SimPort;

	int         FswTlmSocket;
	int         FswCmdSocket;
	int         GndTlmSocket;
	int         GndCmdSocket;
	int         SimSocket;

	pthread_t   FswTlmListenerThread;
	pthread_t   GndCmdListenerThread;
	pthread_t   SimListenerThread;
};
