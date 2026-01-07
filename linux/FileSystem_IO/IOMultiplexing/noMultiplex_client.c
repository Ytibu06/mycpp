#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>



/* 读取终端中的内容，发送给服务端，接收服务端信息，打印在控制台中 */
int main(int argc, const char* argv[])
{
    // ./client fifo1 fifo2

    /* 参数检验 */
    if(argc != 3){
        printf("argc error!\n");
        exit(1);
    }

    /* 确保文件描述符可用 */
    int wfd = open(argv[1], O_WRONLY);
    if(wfd == -1){
        perror("open");
        exit(1);
    }
    int rfd = open(argv[2], O_RDONLY);
    if(rfd == -1){
        perror("open");
        exit(1);
    }
    

    printf("client is connected!\n");

    char buf[1024] = {0};
    while(1){

        /* 清空，读取终端内容 */
        memset(buf, 0, sizeof(buf));
        int read_ret = read(STDIN_FILENO, buf, sizeof(buf));
        if(read_ret < 0){
            perror("read from stdin");
            break;
        }
        
        if(read_ret == 0) {  // 用户可能按了Ctrl+D
            break;
        }

        /* 将终端读取的内容，发送给服务端 */
        int write_ret = write(wfd, buf, read_ret);
        if(write_ret == -1){
            perror("write");
            close(rfd);
            close(wfd);
            exit(1);
        }

        /* 清空，读取服务端发送的信息 */
        memset(buf, 0, sizeof(buf));
        int read_server_ret = read(rfd, buf, sizeof(buf));
        if(read_server_ret == -1){
            perror("read");
            close(rfd);
            close(wfd);
            exit(1);
        }

        /* 读取内容打印在控制台上 */
        if(read_server_ret > 0){
            printf("The client sent %ld bits, which represent: %s", strlen(buf), buf);
        }
        
    }

    return 0;
}