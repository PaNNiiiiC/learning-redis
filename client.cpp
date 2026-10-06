#include "helper.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
void process_cl(int fd) {
    std::string cwbuff="client: Hi";
    write(fd,cwbuff.data(),cwbuff.size());
    char crbuf[64]={};
    ssize_t rsize=read(fd,crbuf,sizeof(crbuf)-1);
    if (rsize<0) die("client read err");
    std::cout<<crbuf<<'\n';
}
int main() {
    int cfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in caddr={};
    caddr.sin_family=AF_INET;
    caddr.sin_port=htons(1234);
    caddr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);

    if (connect(cfd,reinterpret_cast<sockaddr*>(&caddr),sizeof(caddr))<0) die("cl connect err");
    process_cl(cfd);
    close(cfd);
    return 0;
}