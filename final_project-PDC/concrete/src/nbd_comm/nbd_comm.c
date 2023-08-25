
#define _POSIX_C_SOURCE (200809L)
#define BLKSIZE 1024
#define TIMEOUT 15
#define STD stderr

#include <assert.h> 
#include <errno.h>
#include <stddef.h>          /* offsetof */
#include <sys/socket.h>      /* socketpair */
#include <fcntl.h>           /* open */
#include <sys/types.h>       /* fork */
#include <unistd.h>          /* fork */
#include <sys/ioctl.h>       /* ioctl, _IO */
#include <stdlib.h>          /* malloc, free */
#include <string.h>          /* memcpy */
#include <arpa/inet.h>       /* ntohl */

#include "nbd_comm.h"

typedef struct nbd_request nbd_request_t;

typedef struct WRequest
{
    NbdRequest rqst;
    int sock;
    struct nbd_reply rply;
    char buff[1];

}massage_t;

static int write_all(int fd, char* buf, size_t count);
static int read_all(int fd, char *buff, size_t bytes_to_read);
static int InitIoctl(int nbd_fd ,uint64_t size, int sk);
static void InitMessage(nbd_request_t *request, massage_t *massage ,  int sock);




#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a) {
  return a;
}
#else
u_int64_t ntohll(u_int64_t a) {
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll


int NbdDevOpen(const char* dev_name, uint64_t size)
{
    int sp[2];
    int error = 0, nbd =0;
    pid_t pid;

    printf("init start......\n");

    nbd = open(dev_name, O_RDWR);
    if(-1 == nbd)
    {
        fprintf(stderr,"open %s failed \n", dev_name);
        return -1;
    }
    printf("%s opened seccessesfuly\n", dev_name);
    
    /* create 2 sockets and makes the connction btw them by TCP*/
    if(-1 == (error = socketpair(AF_UNIX, SOCK_STREAM, 0, sp)))
    {
        fprintf(stderr,"socketpair failed \n");
        return -1;
    }
    printf("soketpair done\n");

    if(0 < InitIoctl(nbd, size, sp[1]))
    {
        printf("InitIoctl Error!!\n");
        return -1;
    }
    printf("ioctl done\n");

    errno =0;
    if( 0 > (pid = fork()))
    {
        printf("errno: %d\n",errno);
        perror("fork");
    }
    if(pid == 0)
    {
        /* printf("child proc pid:%d \n" , getpid()); */
         /* The child is closing the duplicate sk */
        close(sp[0]);
   
        /* blocking: nbd wait for requasts*/
        if(-1 == ( error = ioctl(nbd, NBD_DO_IT)))
        {
            fprintf(stderr,"NBD_DO_IT error (DO_IT exit from blocking)\n");
            return -1;
        }
    perror("DOIT_OUT: failed\n");
    
    }
 
    /* printf("parent proc. pid:%d \n" , getpid()); */
    /* The parent is closing the dupli sk */
    close(sp[1]);
    
    return sp[0];

}

NbdRequest *NbdRequestRead(int sock)
{
    nbd_request_t nbd_request;
    massage_t *massage =0;
    
   /*  printf("Reading Request........\n"); */

    /* wait for massage from the nbd */
    if(1 == read_all(sock, (char*)&nbd_request, sizeof(nbd_request_t)))
    {
        fprintf(stderr, "NbdRequestRead: read Error\n");
        return NULL;
    }
    /* printf("Reading Request Completed\n"); */
    
    if(nbd_request.magic != htonl( NBD_REQUEST_MAGIC))
    {
        fprintf(stderr, "request_megic Error\n");
        return NULL;
    }

    /* malloc for NbdRequest + Reply + Buffer as a block in single alloction*/
    if(NULL == (massage = malloc(offsetof(massage_t, buff)+ntohl(nbd_request.len))))
    {
        fprintf(stderr, "NbdRequestRead_massage_t\n");
        return NULL;
    }
    
    InitMessage(&nbd_request, massage, sock);
    massage->rqst.dataBuf = massage->buff;
    
    /*  */
    if(massage->rqst.reqType ==  NBD_CMD_WRITE)
    {
        printf("Write data to storage........\n");
        read_all(sock, massage->buff,massage->rqst.dataLen);
        printf("write data Completed \n");
    }

    return (NbdRequest *)massage;
}

void NbdRequestDone(NbdRequest* req, int error)
{
    massage_t *massage = (massage_t *)req;
    size_t len = sizeof(struct nbd_reply);

    massage->rply.error = ntohl(error);
    
    if(massage->rqst.reqType == NBD_CMD_READ)
    {
        printf("Read Request........\n");
        /* if read- wirte to socket the data */
        len += massage->rqst.dataLen; 
    }
    write_all(massage->sock, (char*)&massage->rply, len);
    
    printf("NbdRequestDone.\n");
    
    free(massage);
}

static int read_all(int fd, char *buff, size_t bytes_to_read)
{

    ssize_t bytes_read;

    /* printf("Reading........\n"); */
    while (bytes_to_read >0)
    {
        bytes_read = read (fd, buff, bytes_to_read);

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

static int write_all(int fd, char* buf, size_t count)
{
  ssize_t bytes_written = 0;

    /* printf("Writing........\n"); */
    while (count > 0) {
    bytes_written = write(fd, buf, count);
    
    if(bytes_written < 0)
    {
        printf("writing failed\n");
        return 1;
    }
    buf += bytes_written;
    count -= bytes_written;
  }

   if(count != 0)
    {
        printf("write failed\n");
        return 1;
    }

  return 0;
}

static int InitIoctl(int nbd_fd ,uint64_t size, int sk)
{
    int err=0;
    err += (-1 == ioctl(nbd_fd, NBD_CLEAR_QUE));
    err += (-1 == ioctl(nbd_fd, NBD_CLEAR_SOCK));
    err += (-1 == ioctl(nbd_fd, NBD_SET_BLKSIZE,BLKSIZE));
    err += (-1 == ioctl(nbd_fd, NBD_SET_SIZE, size));
    err += (-1 == ioctl(nbd_fd, NBD_SET_TIMEOUT, 5));
    err += (-1 == ioctl(nbd_fd, NBD_SET_SOCK, sk));


    return err;
}

static void InitMessage(nbd_request_t *request, massage_t *massage ,  int sock)
{
    massage->rply.magic = htonl(NBD_REPLY_MAGIC);
    massage->rply.error = htonl(0);
    memcpy(massage->rply.handle, request->handle, sizeof(massage->rply.handle));

    massage->rqst.dataLen = ntohl(request->len);
    massage->rqst.reqType = ntohl(request->type);
    massage->rqst.offset = ntohll(request->from);
    massage->sock = sock;
}
