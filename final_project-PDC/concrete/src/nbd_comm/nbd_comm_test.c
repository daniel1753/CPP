
#include <stdlib.h>
#include <string.h>


#include "nbd_comm.h"


int main()
{
    static const int StorageSize = 0x100000;
    char* storage = malloc(StorageSize);
    int nbdSock = NbdDevOpen("/dev/nbd0", StorageSize);
    printf("starting....\n");
  /*   sleep(30); */
    if(nbdSock == -1)
    {
        printf("open problem!!!!!!\n");
        exit(1);
    }
   /*  sleep(20); */
    
    while (1)
    {
        NbdRequest* req = NbdRequestRead(nbdSock);

        if(!req)
        {   
            perror("main_req");
        }

        switch (req->reqType)
        {
        case NBD_CMD_READ:
            printf("main - READ\n");
            memcpy(req->dataBuf, storage + req->offset, req->dataLen);
            printf("main - READ done\n");
            break;
        
        case NBD_CMD_WRITE:
            printf("main - WRITE \n");
            memcpy(storage + req->offset, req->dataBuf, req->dataLen);
            printf("main - WRITE done\n");
            break;
        
        default:
            break;
        }

        NbdRequestDone(req, 0);
    }
    
    return 0;
}
