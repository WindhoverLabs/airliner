#include "OBC.hpp"
#include <arpa/inet.h>
#include <error.h>
#include <string.h>
#include <pthread.h>


OBC::OBC()
{
}

OBC::OBC(std::string name,
	     uint16_t    fswTlmPort,
		 std::string fswCmdIP,
	     uint16_t    fswCmdPort,
		 std::string gndTlmIP,
	     uint16_t    gndTlmPort,
	     uint16_t    gndCmdPort,
		 std::string simIP,
	     uint16_t    simPort) :
      Name(name),
      FswTlmPort(fswTlmPort),
      FswCmdIP(fswCmdIP),
      FswCmdPort(fswCmdPort),
      GndTlmIP(gndTlmIP),
      GndTlmPort(gndTlmPort),
      GndCmdPort(gndCmdPort),
      SimIP(simIP),
      SimPort(simPort)
{
    int reuseaddr = 1;

	/* Create all the sockets. */
	FswTlmSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	FswCmdSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	GndTlmSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	GndCmdSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	SimSocket    = socket(AF_INET, SOCK_STREAM, 0);

	/* Set socket options. */
    setsockopt(FswTlmSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    setsockopt(FswCmdSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    setsockopt(GndTlmSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    setsockopt(GndCmdSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    setsockopt(SimSocket,    SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
}



int OBC::Start(void)
{
    struct sockaddr_in address;
    int    rc;

    /* Bind sockets. */
    bzero((char *) &address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port        = htons(FswTlmPort);
    if(bind(FswTlmSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cerr << Name << ": FSW:TLM Bind socket failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    address.sin_port        = 0;
    if(bind(FswCmdSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cerr << Name << ": FSW:CMD Bind socket failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    address.sin_port        = 0;
    if(bind(GndTlmSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cerr << Name << ": GND:TLM Bind socket failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    address.sin_port        = htons(FswCmdPort);
    if(bind(GndCmdSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cerr << Name << ": GND:CMD Bind socket failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    address.sin_port        = htons(SimPort);
    if(bind(SimSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cerr << Name << ": SIM Bind socket failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    /* Create listener threads. */
    rc = pthread_create(&FswTlmListenerThread, 0, &OBC::c_FswTlmListener, this);
    if(rc < 0)
    {
        std::cerr << Name << ": FSW:TLM listener thread creation failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    rc = pthread_create(&GndCmdListenerThread, 0, &OBC::c_GndCmdListener, this);
    if(rc < 0)
    {
        std::cerr << Name << ": GND:CMD listener thread creation failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

    rc = pthread_create(&SimListenerThread, 0, &OBC::c_SimListener, this);
    if(rc < 0)
    {
        std::cerr << Name << ": SIM listener thread creation failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

	return 0;
}



void *OBC::c_FswTlmListener(void *arg)
{
	OBC *myOBC = (OBC*)arg;

	myOBC->FswTlmListener();
}


void *OBC::c_GndCmdListener(void *arg)
{
	OBC *myOBC = (OBC*)arg;

	myOBC->GndCmdListener();
}


void *OBC::c_SimListener(void *arg)
{
	OBC *myOBC = (OBC*)arg;

	myOBC->SimListener();
}



void OBC::FswTlmListener(void)
{
    std::cout << Name << " FSW:TLM started\n";
}


void OBC::GndCmdListener(void)
{
    std::cout << Name << " GND:CMD started\n";
}


void OBC::SimListener(void)
{
    std::cout << Name << " SIM started\n";
}
