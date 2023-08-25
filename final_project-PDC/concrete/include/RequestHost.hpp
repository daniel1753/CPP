/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: RequestHost.cpp					
 * ******************************/

#pragma once

#include <set> /* set */
#include <memory> /* shared_ptr */
#include <unordered_map> /* unordered_map */
#include <sys/socket.h>      /* socket ,sendmsg*/
#include <sys/types.h>       /* sendmsg */

#include "config.hpp"
#include "ThreadPool.hpp" /* ThreadPool , Task , Future*/
#include "nbd_comm.h" 
#include "Transmitter.hpp"



namespace ilrd
{

class RequestHost
{

public:

    static std::shared_ptr<RequestHost> Create(int *sokets);
    void Slice(uint32_t *info);   
    static uint32_t Peek(int sock, int flag = 0);
    int ReadRply(int sock);
    uint32_t GetId();

private:
    explicit RequestHost() ;
   
    uint32_t m_request_uid;
    uint32_t m_first_freg;
    NbdRequest *m_nbq_req;
    
    std::vector<int> m_sockets;
    std::unordered_map<int, std::shared_ptr<Transmitter> > m_transmitters;

};




}

