#include <sys/socket.h>
#include <netinet/ip.h>

int main() {

    int server_fd=socket(AF_INET,SOCK_STREAM,0);
    int opt=1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    
    return 0;
}