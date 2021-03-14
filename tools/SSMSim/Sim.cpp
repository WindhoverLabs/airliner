#include "Sim.hpp"
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



Sim::Sim()
{
}

Sim::Sim(std::string ip,
         uint16_t    port) :
      IP(ip),
      Port(port)
{
    int reuseaddr = 1;

	/* Create the socket. */
    Socket = socket(AF_INET, SOCK_STREAM, 0);

	/* Set socket options. */
    setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
}



int Sim::Start(void)
{
    int    rc;

    /* Create listener threads. */
    rc = pthread_create(&ListenerThread, 0, &Sim::c_Listener, this);
    if(rc < 0)
    {
        std::cerr << "SIM listener thread creation failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

	return 0;
}


void *Sim::c_Listener(void *arg)
{
	Sim *mySim = (Sim*)arg;

	mySim->Listener();
}


void Sim::Listener(void)
{
	int rc;
	struct sockaddr_in s_addr;
	bzero((char *) &s_addr, sizeof(s_addr));
	s_addr.sin_family      = AF_INET;
	s_addr.sin_addr.s_addr = inet_addr(IP.c_str());
	s_addr.sin_port        = htons(Port);
	static char buffer[32578];
	size_t size;

	/* Connect to the sim. */
	rc = connect(Socket, (struct sockaddr*)&s_addr, sizeof(s_addr));
	if (rc < 0)
	{
		std::cout << "SIM connect failed. (" << errno << ") '" << strerror(errno) << "'\n";
		exit(-1);
	}

    std::cout << "SIM listener started\n";

	while(1)
	{
		/* Wait for data from the sim. */
		size = sizeof(buffer);
		rc = read(Socket, (char *)buffer, size);
		if (rc < 0)
		{
			std::cout << "SIM read failed. (" << errno << ") '" << strerror(errno) << "'\n";
			sleep(1);
		}
		else
		{
			/* TODO: Send this to the flight software */
		}

	}
}



void Sim::SendMsg(char *buffer, uint16_t size)
{
    if(buffer == 0)
    {
    	/* TODO: Log this. */
    	return;
    }

    if(size == 0)
    {
    	/* TODO: Log this. */
    	return;
    }

    write(Socket, buffer, sizeof(size));
}
