#include "Sim.hpp"
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>



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
    Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* Set socket options. */
    setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
}



int Sim::Start(void)
{
    struct sockaddr_in address;
    int    rc;

    /* Bind socket. */
    bzero((char *) &address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port        = htons(Port);
    if(bind(Socket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cerr << "SIM Bind socket failed. (" << errno << ") " << strerror(errno) << std::endl;
        return -1;
    }

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
    std::cout << "SIM listener started\n";
}
