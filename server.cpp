#include "helper.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>


void process_in(int fd) {
    char rbuf[64]={};
    ssize_t rsize=read(fd,rbuf,sizeof(rbuf)-1);
    if (rsize<0) die("read err");
    std::cout<<rbuf<<'\n';

    std::string wbuf="server : it's me PaNNiiiiC";
    write(fd,wbuf.data(),wbuf.size());
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
        process_in(conn_fd); // read/write;
        close(conn_fd);
    }

    return 0;
}