#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

/* 终端可读时：读取内容，写入管道通知服务端，服务端可读时：读取管道内容打印在终端中 */
int main(int argc, char *argv[])
{
    // ./client fifo1 fifo2

    /* 参数检验 */
    if (argc != 3)
    {
        printf("argc error! Usage: %s <read_fifo> <write_fifo>\n", argv[0]);
        exit(1);
    }

    /* 确保文件描述符可用 */
    int wfd = open(argv[1], O_WRONLY);
    if (wfd == -1)
    {
        perror("open");
        exit(1);
    }
    int rfd = open(argv[2], O_RDONLY);
    if (rfd == -1)
    {
        perror("open");
        exit(1);
    }

    printf("client is connected!\n");

    char buf[1024] = {0};
    fd_set rdset;
    int maxfd = (rfd > STDIN_FILENO) ? rfd : STDIN_FILENO;  //获取最大文件描述符
    while (1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(rfd, &rdset);
        int ret = select(maxfd + 1, &rdset, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("select");
            close(rfd);
            close(wfd);
            exit(1);
        }

        /* 终端可读(终端输入内容):读取终端，写入管道中*/
        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            /* 清空，读取终端内容 */
            memset(buf, 0, sizeof(buf));
            int read_ret = read(STDIN_FILENO, buf, sizeof(buf));
            if (read_ret < 0)
            {
                perror("read from stdin");
                break;
            }

            if (read_ret == 0)
            { // 用户可能按了Ctrl+D
                break;
            }

            /* 将终端读取的内容，发送给服务端 */
            int write_ret = write(wfd, buf, read_ret);
            if (write_ret < 0)
            {
                perror("write");
                close(rfd);
                close(wfd);
                exit(1);
            }
        }

        /* 管道可读(服务端发送信息)：读取内容，打印在控制台上*/
        if (FD_ISSET(rfd, &rdset))
        {
            /* 清空buf，读取服务端发送的信息 */
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

            /* 读取内容打印在控制台上 */
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