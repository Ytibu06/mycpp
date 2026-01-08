#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

/* 终端可读时，读取内容，写入管道发送给客户端。管道可读时读取内容，打印在终端上*/
int main(int argc, char *argv[])
{
    // ./client fifo1 fifo2

    /* 参数检验*/
    if (argc != 3)
    {
        printf("argc error!");
        exit(1);
    }

    /* 确保存在文件描述符 */
    int rfd = open(argv[1], O_RDONLY);
    if (rfd == -1)
    {
        perror("open");
        exit(1);
    }
    int wfd = open(argv[2], O_WRONLY);
    if (wfd == -1)
    {
        perror("open");
        exit(1);
    }

    printf("server is connected!\n");

    char buf[1024] = {0};
    fd_set rdset; // fd_set申请内存
    int maxfd = (rfd > STDIN_FILENO) ? rfd : STDIN_FILENO;  //获取最大的文件描述符
    while (1)
    {
        FD_ZERO(&rdset); // 清空
        FD_SET(rfd, &rdset);    //写入监控的文件描述符
        FD_SET(STDIN_FILENO, &rdset);
        struct timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        int ret = select(maxfd + 1, &rdset, NULL, NULL, &timeout);
        if (ret < 0)
        {
            perror("select");
            close(rfd);
            close(wfd);
            exit(1);
        }

        /* 设置超时时间，超时发送提醒，根据实际情况进行处理*/
        if (ret == 0)
        {
            printf("timeout!\n");
            time_t now = time(NULL);
            printf("%s", ctime(&now));
            continue;
        }

        /* 终端可读(终端输入内容):读取终端，写入管道中*/
        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            memset(buf, 0, sizeof(buf));
            int read_ret = read(STDIN_FILENO, buf, sizeof(buf));
            if (read_ret < 0)
            {
                perror("read from stdin");
                break;
            }

            if (read_ret == 0)
            {
                break;
            }

            int write_ret = write(wfd, buf, read_ret);
            if (write_ret < 0)
            {
                perror("write");
                close(rfd);
                close(wfd);
                exit(1);
            }
        }

        /* 管道可读，读取内容，打印在终端中*/
        if (FD_ISSET(rfd, &rdset))
        {
            /* 清空，读取客户端信息 */
            memset(buf, 0, sizeof(buf));
            int read_ret = read(rfd, buf, sizeof(buf) - 1);
            if (read_ret < 0)
            {
                perror("read");
                close(rfd);
                close(wfd);
                exit(1);
            }

            if (read_ret == 0)
            {
                break;
            }

            if (read_ret > 0)
            {
                printf("The client sent %ld bits, which represent: %s", strlen(buf), buf);
            }
        }
    }

    close(rfd);
    close(wfd);

    return 0;
}