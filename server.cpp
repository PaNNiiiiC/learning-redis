#include "helper.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>
const int mx_sz=4096;
int request(int fd) {

    // receiving
    char rbuf[4+mx_sz];
    errno=0;
    int32_t rv=read_all(fd,rbuf,4);
    if (rv) {
        printf(errno==0 ? "EOF" : "read_all error");
        return rv;
    }
    int32_t sz=0;
    memcpy(&sz,rbuf,4);
    if (sz > mx_sz) {
        printf("msg too long");
        return -1;
    }
    rv=read_all(fd,&rbuf[4],sz);
    if (rv) {
        printf("read_all error");
        return rv;
    }
    //responding
    char wbuf[4+mx_sz];
    char pref[]="your message starting with ";
    char suff[]="was sucessfully received";
    size_t ans_sz=strlen(pref)+strlen(suff)+1;
    memcpy(wbuf,&ans_sz,4);
    memcpy(&wbuf[4],pref,strlen(pref));
    memcpy(&wbuf[4+strlen(pref)],&rbuf[4],1);
    memcpy(&wbuf[5+strlen(pref)],suff,strlen(suff));
    return write_all(fd,wbuf,ans_sz);
}

int main() {

    //socket init
    int server_fd=socket(AF_INET,SOCK_STREAM,0);
    int opt=1;
    if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))) die("socket limit");

    //binding
    sockaddr_in addr={};
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(0);
    addr.sin_port=htons(1234);
    if (bind(server_fd,reinterpret_cast<const sockaddr *>(&addr),sizeof(addr))) die("socket unavailiable");

    //listening
    if (listen(server_fd,SOMAXCONN)) die("can't listen");
    while (true) {
        //accepting
        sockaddr_in cl_addr={};
        socklen_t len=sizeof(cl_addr);
        int conn_fd=accept(server_fd,reinterpret_cast<sockaddr *>(&cl_addr),&len);
        if (conn_fd<0) continue;
        request(conn_fd); // read/write;
        close(conn_fd);
    }
    return 0;
}