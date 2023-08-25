/*********************************
 * Reviewer:  		
 * Author: Daniel Binyamin			
 * File: Dispatcher.cpp					
 * ******************************/

#include <iostream>
#include "Dispatcher.hpp"
#include "Singleton.hpp"
#include "Factory.hpp"
#include "Command.hpp"

namespace ilrd
{

int RequestDispatcher::NbdRequestHandler(int *socks)
{
    Singleton<Factory<Command,int,std::shared_ptr<message_info>>> factory;
    factory->Add<Write>(NBD_CMD_WRITE);
    factory->Add<Read>(NBD_CMD_READ);

    std::shared_ptr<RequestHost>req = RequestHost::Create(socks);
    m_activeRequest.insert(std::make_pair(req->GetId(),req));
    
    return 0;
}

int RequestDispatcher::IotRequestHandler(int sock)
{
    uint32_t reqUid = RequestHost::Peek(sock);

    if(static_cast<uint32_t>(-1) == reqUid ){perror("peek:");}
    
    if(m_activeRequest.at(reqUid)->ReadRply(sock) == 6)
    {
        m_activeRequest.erase(reqUid);
    }

    return 0;
}

};