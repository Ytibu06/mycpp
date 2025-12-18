#include <Cfun.h>

void client(const char *ip, int port){

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("socket");
        return;
    }
    int conn_ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if(conn_ret < 0){
        perror("connect");
        return;
    }

    while(1){
        char buf[1024] = {0};

        int sen_ret = send(sockfd, "hello world\n", 12, 0);
        if(sen_ret < 0){
            perror("send");
            exit(1);
        }

        int recv_ret = recv(sockfd, buf, sizeof(buf)-1, 0);
        if(recv_ret <= 0){
            if(recv_ret < 0){
                perror("recv");
                exit(1);
            }
            break;
        }

        buf[recv_ret] = '\0';
        printf("client: %s\n", buf);

        sleep(1);
    }

    close(sockfd);

}

int main(int argc, char *argv[]){
    
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    
    client(ip, port);
}