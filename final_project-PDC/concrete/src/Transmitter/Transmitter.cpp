/*********************************
 * Reviewer:  		
 * Author: Daniel Binyamin			
 * File: Transmitter.cpp					
 * ******************************/

#include "Transmitter.hpp"
#include "Factory.hpp"
#include "Singleton.hpp"


namespace ilrd
{
static int read_all(int fd, char *buff, size_t bytes_to_read,  int flags =0);

Dispatcher<AtlasHeader*> Transmitter::m_disp;

Transmitter::Transmitter(NbdRequest *NbdReq):m_NbdReq(NbdReq){}

int Transmitter::SendTask(std::shared_ptr<message_info> msgInfo)
{
    for(uint32_t i = msgInfo->header.m_fregIdx;
        i < msgInfo->freg_count;
        i+=IOT_COUNT)
    {
        m_iot_set.insert(i);
    }

    //TODO:: make factory static and not singelton
    Singleton<Factory<Command,int,std::shared_ptr<message_info> >> factory;

    //run until the last fregidx add iot_count*fregsize
    for(; msgInfo->header.m_fregIdx < msgInfo->freg_count; 
          msgInfo->header.m_fregIdx += IOT_COUNT ,
          msgInfo->header.m_iotOffset += msgInfo->header.m_len)
    {

        // TODO:make command per task and not per fregment
        std::shared_ptr<Command> com = factory->Create(msgInfo->header.m_type, 
                                                       msgInfo);
        com->Build(msgInfo);
        m_disp.Notify(&msgInfo->header);
        com->Send(msgInfo);
        
    }
    return 0;
}

int Transmitter::ReplyHandler(int sock)
{
    AtlasHeader header;
    
    if(1 == read_all(sock, 
                     reinterpret_cast<char*>(&header), 
                     sizeof(AtlasHeader)))
    {
        fprintf(stderr, "Iot_handler: read Error\n");
        return -1;
    }

    if(m_NbdReq->reqType == NBD_CMD_READ)
    {
        if(-1 == read_all(sock, (m_NbdReq->dataBuf)+ 
                       (header.m_fregIdx *header.m_len), header.m_len))
                       {
                            perror("read_all");
                       }
    }

    m_iot_set.erase(header.m_fregIdx);
    if(m_iot_set.empty())
    {
        return 6;
    }
    return 0;
}

void Transmitter::Add(const Callback<AtlasHeader *> &callback)
{
    m_disp.Add(callback);
}

static int read_all(int fd, char *buff, size_t bytes_to_read,  int flags)
{
    ssize_t bytes_read;

    while (bytes_to_read >0)
    {
        bytes_read = recv(fd, buff, bytes_to_read, flags);
        if(bytes_read < 0)
        {
            printf("reading failed\n");
            return 1;
        }
        
        buff += bytes_read;
        bytes_to_read -=bytes_read;
    }

    if(bytes_to_read != 0)
    {
        printf("reading failed\n");
        return 1;
    }
    return 0;
}

}