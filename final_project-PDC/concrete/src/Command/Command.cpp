/*********************************
 * Reviewer:  		
 * Author: Daniel Binyamin			
 * File: Command.cpp					
 * ******************************/

#include "Command.hpp"
#include <cstring>           /* memset */


namespace ilrd
{
void BuildBase(std::shared_ptr<message_info> m_msgInfo);
/********************************** Command **********************************/    

Command::Command(std::shared_ptr<message_info> msgInfo){}

int Command::Send(std::shared_ptr<message_info> msgInfo)
{
    return (sendmsg(msgInfo->fd, &msgInfo-> m_msg, 0));
}

/*********************************** Write ***********************************/    

Write::Write(std::shared_ptr<message_info> msgInfo): Command(msgInfo){}

void Write::Build(std::shared_ptr<message_info> msgInfo)
{
    
   BuildBase(msgInfo);  
    msgInfo->m_msg.msg_iov[1].iov_base = msgInfo->NbdReq->dataBuf + 
                    msgInfo->header.m_len * 
                    msgInfo->header.m_fregIdx;
    msgInfo->m_iov[1].iov_len = msgInfo->header.m_len;
    msgInfo->m_msg.msg_iovlen = 2;
  
}

/*********************************** Read ***********************************/    

Read::Read(std::shared_ptr<message_info> msgInfo): Command(msgInfo){}

void Read::Build(std::shared_ptr<message_info> msgInfo)
{
    BuildBase(msgInfo);
}

void BuildBase(std::shared_ptr<message_info> msgInfo)
{
    memset(&msgInfo->m_msg, 0, sizeof(msgInfo->m_msg));
    msgInfo->m_msg.msg_iovlen = 1;
    msgInfo->m_iov[0].iov_base = &(msgInfo->header);
    msgInfo->m_iov[0].iov_len = sizeof(AtlasHeader);
    msgInfo->m_msg.msg_iov =msgInfo->m_iov; 
}

} // namespace ilrd
