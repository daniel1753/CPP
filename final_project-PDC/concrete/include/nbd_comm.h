
/*********************************
 * Reviewer: Daniel-natan				
 * Author: Daniel Binyamin			
 * File: nbd_comm.h					
 * ******************************/

#ifndef __NBD_COMM_H__
#define __NBD_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <linux/nbd.h>       /* nbd, reply */
#include <stdio.h>           /* printf, perror */
#include <sys/socket.h>      /* socket ,sendmsg , iov ,msg*/


typedef struct nbd
{
    uint32_t reqType;
    uint32_t dataLen;
    uint64_t offset;
    char *dataBuf;
    
}NbdRequest;

typedef struct AtlasHeader
{
    uint32_t m_requestUid;
    uint32_t m_fregIdx;
    uint32_t m_alarmUid;
    uint32_t m_iotOffset;
    uint32_t m_type;
    uint32_t m_len;

}AtlasHeader;

typedef struct info
{
    int fd;
    uint32_t LastFragIdx;  
    uint32_t freg_count;  
    AtlasHeader header;
    NbdRequest *NbdReq;
    struct iovec m_iov[2];
    struct msghdr m_msg;

}message_info;


int NbdDevOpen(const char* dev_name, uint64_t size);

NbdRequest *NbdRequestRead(int sock);

void NbdRequestDone(NbdRequest* req, int error);



#ifdef __cplusplus
}
#endif


#endif 