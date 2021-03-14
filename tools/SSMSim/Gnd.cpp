#include "Gnd.hpp"
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>



Gnd::Gnd()
{
}

Gnd::Gnd(std::string tlmIP,
         uint16_t    tlmPort,
         uint16_t    cmdPort) :
      TlmIP(tlmIP),
      TlmPort(tlmPort),
      CmdPort(cmdPort)
{
    int reuseaddr = 1;

	/* Create all the sockets. */
    TlmSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    CmdSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* Set socket options. */
    setsockopt(TlmSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    setsockopt(CmdSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
}



int Gnd::Start(void)
{
    struct sockaddr_in address;
    int    rc;

    /* Bind sockets. */
    bzero((char *) &address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port        = htons(CmdPort);
    if(bind(CmdSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cerr << "GND TLM Bind socket failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    address.sin_port        = 0;
    if(bind(TlmSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cerr << "GND TLM Bind socket failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    /* Create listener threads. */
    rc = pthread_create(&ListenerThread, 0, &Gnd::c_Listener, this);
    if(rc < 0)
    {
        std::cerr << "GND listener thread creation failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

	return 0;
}


void *Gnd::c_Listener(void *arg)
{
	Gnd *mySim = (Gnd*)arg;

	mySim->Listener();
}


void Gnd::Listener(void)
{
    std::cout << "GND listener started\n";
}

