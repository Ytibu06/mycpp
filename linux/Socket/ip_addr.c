#include <Cfun.h>
#include <sys/socket.h>
int main()
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr =  inet_addr("127.0.0.1");
    //inet_aton("127.0.0.1", &server_addr.sin_addr);
    //inet_aton();
}