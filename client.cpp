#include "helper.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <cstring>
const int mx_sz=4096;
int32_t cl_request(int fd, char* msg, size_t msg_sz) {
    char wbuf[4+mx_sz];
    memcpy(wbuf,&msg_sz,4);
    memcpy(&wbuf[4],msg,msg_sz);
    int32_t rv=write_all(fd,wbuf,msg_sz+4);
    if (rv<0) {
        printf("write_all error");
        return -1;
    }

    errno=0;
    char rbuf[4+mx_sz];
    rv=read_all(fd,rbuf,4);
    if (rv<0) {
        printf(errno==0 ? "EOF" : "read_all error");
        return -1;
    }
    size_t sz=0;
    memcpy(&sz,rbuf,4);
    if (sz>mx_sz) {
        printf("msg too long");
        return -1;
    }
    rv=read_all(fd,&rbuf[4],sz);
    if (rv<0) {
        printf("write_all error 2");
        return -1;
    }
    printf("%s",&rbuf[4]);
    return 0;
}
int main() {

    //server connect
    int cfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in saddr={};
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    saddr.sin_port=htons(1234);
    if (connect(cfd,reinterpret_cast<sockaddr*>(&saddr),sizeof(saddr))<0) die("cl connect err");

    char msg1[]="1 Hello";
    int32_t rv=cl_request(cfd,msg1,strlen(msg1)); // read/write
    if (rv<0) {close(cfd);return 0;}
    char msg2[]="2 Hello";
    rv=cl_request(cfd,msg2,strlen(msg2));
    close(cfd);
    return 0;
}