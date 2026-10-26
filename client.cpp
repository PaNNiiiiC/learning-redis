#include "helper.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
void process_cl(int fd) {
    std::string cwbuff;
    std::cin>>cwbuff;
    write(fd,cwbuff.data(),cwbuff.size());
    char crbuf[64]={};
    ssize_t rsize=read(fd,crbuf,sizeof(crbuf)-1);
    if (rsize<0) die("client read err");
    else if (rsize==0) die("EOF");
    std::cout<<"server says: "<<crbuf<<'\n';
}
int main() {

    //server connect
    int cfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in saddr={};
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    saddr.sin_port=htons(1234);
    if (connect(cfd,reinterpret_cast<sockaddr*>(&saddr),sizeof(saddr))<0) die("cl connect err");
    sockaddr_in temp={};
    socklen_t tmpsz=sizeof(temp);
    process_cl(cfd); // read/write
    close(cfd);
    return 0;
}