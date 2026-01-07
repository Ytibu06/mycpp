#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


//读取客户端发送的内容，拷贝内容后处理，发送给客户端
int main(int argc, const char* argv[])
{
    // ./client fifo1 fifo2
    
    /* 参数检验*/
    if(argc != 3){
        printf("argc error!\n");
        exit(1);
    }

    /* 确保存在文件描述符 */
    int rfd = open(argv[1], O_RDONLY);
    if(rfd == -1){
        perror("open");
        exit(1);
    }
    int wfd = open(argv[2], O_WRONLY);
    if(wfd == -1){
        perror("open");
        exit(1);
    }
    

    printf("server is connected!\n");

    char buf[1024] = {0};
    char trans[1024] = {0};
    while(1)
    {
        /* 清空，读取客户端信息 */
        memset(buf, 0, sizeof(buf));
        int read_client_ret = read(rfd, buf, sizeof(buf));
        if(read_client_ret < 0){
            perror("read");
            close(rfd);
            close(wfd);
            exit(1);
        }

        /* 处理客户端信息 */
        if(read_client_ret > 0){
            printf("The client sent %ld bits, which represent: %s", strlen(buf), buf);
        }


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

    }

    return 0;
}