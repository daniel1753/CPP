/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: 			
 * ******************************/

#include <cstdlib>      /* malloc */
#include <cstring>      /* memcpy */
#include <iostream>     /* cout */
#include <sys/types.h>  /* sendmsg */
#include <sys/socket.h> /* socket ,sendmsg*/
#include <netinet/in.h> /* sockaddr_in*/
#include <arpa/inet.h>  /* inet_addr*/
#include <unistd.h>     /* read */

#include "nbd_comm.h"
#include "reactor.hpp"
#include "function.hpp"
#include "select.hpp"
#include "config.hpp"
#include "Dispatcher.hpp"
#include "Transmitter.hpp"

enum FDS{NBD,IOT};

using namespace ilrd;


static int TCPConnectToServer(int port_num, int addr);

class Encryption
{

public:
    Encryption(const Function<void(AtlasHeader*)>& func);

    Callback<AtlasHeader*> callback;

};

void Donothing(AtlasHeader *at)
{
    std::cout<<"Encryption activate " << at->m_fregIdx<< std::endl;

}

int main()
{
    int iot_port = 29000;

    static const int StorageSize = 0x10000000;
    int sockets[IOT_COUNT + 1];
    Reactor<Select> reac;
    RequestDispatcher ReqDisp;
    Function<void(AtlasHeader*)>func(&Donothing);
    Encryption encrip(func);

    Transmitter::Add(encrip.callback);

    std::cout << "Starting....\n";

    sockets[NBD] = NbdDevOpen("/dev/nbd1", StorageSize);
    if (sockets[NBD] == -1)
    {
        std::cout << "main: open error....\n";
        return -1;
    }

    Function<int(void)> nbd_func(&RequestDispatcher::NbdRequestHandler,
                                 &ReqDisp, sockets);
    reac.RegisterReadHandler(sockets[NBD], nbd_func);

    for (uint32_t i = 1; i < IOT_COUNT + 1; ++i, ++iot_port)
    {
        sockets[i] = TCPConnectToServer(iot_port, inet_addr("127.0.0.1"));
        if (sockets[i] == -1)
        {
            perror("iots_ConnectToServer");
        }

        Function<int(void)> iot_func(&RequestDispatcher::IotRequestHandler,
                                     &ReqDisp, sockets[i]);
        reac.RegisterReadHandler(sockets[i], iot_func);
    }
    Transmitter::Add(encrip.callback);
    reac.Run();

    return 0;
}

int TCPConnectToServer(int port_num, int addr)
{
    int sockfd;
    struct sockaddr_in servaddr;

    /* Creating socket file descriptor */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed");
        return -1;
    }
    printf("Socket successfully created.. port: %d\n", port_num);
    memset(&servaddr, 0, sizeof(servaddr));

    /* Filling server information */
    servaddr.sin_family = AF_INET; /* IPv4 */
    servaddr.sin_addr.s_addr = addr;
    servaddr.sin_port = htons(port_num);

    if (connect(sockfd,
                reinterpret_cast<struct sockaddr *>(&servaddr),
                sizeof(struct sockaddr_in)) != 0)
    {
        printf("connection with the server failed...\n");
        return -1;
    }
    printf("connected to the server..\n");

    return sockfd;
}

Encryption::Encryption(const Function<void(AtlasHeader *)> &func): callback(func)
{
}




