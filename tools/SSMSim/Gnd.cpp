#include "Gnd.hpp"
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>



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
	static  uint8_t buffer[32578];
	uint32_t  size = sizeof(buffer);

    std::cout << "GND listener started\n";

    while(1)
    {
    	int rc = 0;

    	/* Wait for a command from the ground. */
    	rc = recv(CmdSocket,
    					   (char *)buffer,
    					   (size_t)&size, 0);
    	if(rc <= 0)
    	{
    	    std::cout << "GND recv failed. (" << errno << ") '" << strerror(errno) << "'\n";
    	    sleep(1);
    	}
    	else
    	{
    		if(size > 0)
    		{
    			/* TODO: Forward to the flight software. */
    		}
    	}
    }
}



void Gnd::SendTlm(const uint8_t *buffer, uint16_t size)
{
	int rc;

	if(buffer == 0)
	{
		/* TODO: Log this. */
		return;
	}

	if(size < 0)
	{
		/* TODO: Log this. */
		return;
	}

	/* Send the command to the flight software. */
	struct sockaddr_in s_addr;
	bzero((char *) &s_addr, sizeof(s_addr));
	s_addr.sin_family      = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr(TlmIP.c_str());
    s_addr.sin_port        = htons(TlmPort);
    rc = sendto(TlmSocket, (char *)buffer, size, 0,
                (struct sockaddr *) &s_addr,
                sizeof(s_addr));
    if (rc < 0)
    {
        std::cout << "GND sendto failed. (" << errno << ") '" << strerror(errno) << "'\n";
        sleep(1);
	}
}
