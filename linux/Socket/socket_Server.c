#include <Cfun.h>

void server(const char *ip, int port)
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);


    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockFd < 0){
        perror("socket");
        exit(1);
    }
    int ret = bind(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        perror("bind");
        exit(1);
    }

    while (1)
    {
        sleep(1);
        char buf[1024] = {0};

        ret = listen(sockFd, 5);
        if(ret < 0){
            perror("listen");
            exit(1);
        }

        /* 接收客户端连接，并对客户端ipPort存储*/
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientFd = accept(sockFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if(clientFd < 0){
            perror("accept");
            exit(1);
        }

        send(clientFd, "hello world", 11, 0);

        recv(clientFd, buf, sizeof(buf), 0);
        printf("client ip: %s port: %d , message is: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buf);
        
        close(clientFd);
    }

    close(sockFd);
    
    
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    server(ip, port);
    return 0;
}