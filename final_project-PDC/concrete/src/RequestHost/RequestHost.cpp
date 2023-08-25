/*********************************
 * Reviewer:  		
 * Author: Daniel Binyamin			
 * File: RequestHost.cpp					
 * ******************************/
#include <cstring>           /* memset */
#include <unistd.h>          /* read */

#include "RequestHost.hpp"
#include "function.hpp"
#include "Singleton.hpp"


#define MIN(a, b) ((a) < (b) ? (a) : (b))


namespace ilrd
{
static int read_all(int fd, char *buff, size_t bytes_to_read,  int flags = 0);

RequestHost::RequestHost(){}

uint32_t RequestHost::GetId()
{
    return m_request_uid;
}

uint32_t RequestHost::Peek(int sockfd, int flag)
{
    struct identifiers
    {
        uint32_t reqUid;
        uint32_t fregIdx;
    }id;
    
    if(-1 == read_all(sockfd,
                      reinterpret_cast<char*> (&id), 
                      sizeof(id), 
                      MSG_PEEK))
    {
        perror("RequestHost::Peek");
        return -1;
    }

    return (flag == REQ_UID) ? id.reqUid : id.fregIdx;
}

/*  calculate the num of slices and finds the where to store the firs slice:
    first iot,freg offset.
    allso set the fregs idx in set
*/
void RequestHost::Slice(uint32_t *info)
{
    // num of fregments
    info[FREG_COUNT] = m_nbq_req->dataLen / FREG_SIZE;
    
    m_first_freg = m_nbq_req->offset / FREG_SIZE;
    
    // firstIot
    info[IOT] = m_first_freg % IOT_COUNT;
    
    // firstFregOffset
    info[FREG_OFFSET] = m_first_freg /IOT_COUNT * FREG_SIZE;

}

/* 
    Create a request obj (can be call before constract req obj)
    using threadpool to send message to the iots
*/
std::shared_ptr<RequestHost> RequestHost::Create(int *sokets)
{

    static uint32_t reqUid =0;
    Singleton<ThreadPool> pool;
    uint32_t info[3];

    RequestHost *req_host= new RequestHost;
    req_host->m_nbq_req = NbdRequestRead(sokets[0]);

    req_host->m_request_uid = reqUid;
    ++reqUid;
    
    req_host->Slice(info);
    const uint32_t last_freg = req_host->m_first_freg + info[FREG_COUNT];

    uint32_t nMsgs = MIN(IOT_COUNT, info[FREG_COUNT]);
    for(uint32_t fregIdx= 0 ; fregIdx< nMsgs ; ++fregIdx)
    {

        //aloc shered ptr of msg_info
        std::shared_ptr<message_info> msgInfos(new message_info);

        msgInfos->header.m_requestUid =req_host->m_request_uid;
        msgInfos->header.m_fregIdx = fregIdx;
        msgInfos->header.m_iotOffset = info[FREG_OFFSET];
        msgInfos->header.m_type = req_host->m_nbq_req->reqType;
        msgInfos->header.m_len = FREG_SIZE;

        msgInfos->NbdReq = req_host->m_nbq_req;

        msgInfos->fd = sokets[info[IOT]+1]; 
        msgInfos->LastFragIdx = last_freg;
        msgInfos->freg_count = info[FREG_COUNT];

        std::shared_ptr<Transmitter>trans(new Transmitter(req_host->m_nbq_req));
        req_host->m_transmitters[info[IOT]] = trans;

        //send msg async
        Function<int(void)> Send(&Transmitter::SendTask, trans.get(), msgInfos);
        pool->Async(Send); 

        (++info[IOT])%= IOT_COUNT;
        
        if(info[IOT] == 0)
        {
            info[FREG_OFFSET] += FREG_SIZE;
        }    
    }
    
    return std::shared_ptr<RequestHost>(req_host);
}


int RequestHost::ReadRply(int sock)
{
    uint32_t curr_iot = ((m_nbq_req->offset/FREG_SIZE % IOT_COUNT) + 
                          Peek(sock,FREG_IDX)) %
                          IOT_COUNT;

    if( 6 == (m_transmitters[curr_iot]->ReplyHandler(sock)))
    {
        m_transmitters.erase(curr_iot);
        
        if(m_transmitters.empty())
        {
            NbdRequestDone(m_nbq_req, 0);
            return 6; 
        }
    }


    return 0;
}



static int read_all(int fd, char *buff, size_t bytes_to_read, int flags)
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
