#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/*
    创建两个单工通信的管道，用于服务端和客户端通信，
    设置select监听管道，设置客户端超时时间20秒，
    无论读取到终端还是管道中的终止符，服务端就会退出
*/

int main(int argc, char *argv[])
{
    int rfd = open("/tmp/fifo1", O_RDONLY);
    if (rfd < 0)
    {
        perror("open");
        exit(1);
    }
    int wfd = open("/tmp/fifo2", O_WRONLY);
    if (wfd < 0)
    {
        perror("open");
        exit(1);
    }

    fd_set rdset;
    int maxfd = rfd > STDIN_FILENO ? rfd : STDIN_FILENO;
    time_t lastActive = time(NULL);
    time_t curentTime;
    while (1)
    {
        FD_ZERO(&rdset);
        FD_SET(rfd, &rdset);
        FD_SET(STDIN_FILENO, &rdset);
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        curentTime = time(NULL);
        int ret = select(maxfd + 1, &rdset, NULL, NULL, &timeout);
        if (ret < 0)
        {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            char buf[1024] = {0};
            int ret = read(STDIN_FILENO, buf, sizeof(buf));
            if (ret <= 0)
            {
                perror("read");
                break;
            }
            write(wfd, buf, ret);
        }

        if (FD_ISSET(rfd, &rdset))
        {
            char buf[1024] = {0};
            lastActive = time(NULL);
            int ret = read(rfd, buf, sizeof(buf));
            if (ret <= 0)
            {
                printf("The Client Connect is closed!\n");
                break;
            }
            printf("recevive Client: %s", buf);
        }

        if (curentTime - lastActive >= 20)
        {
            printf("The Client Connect is timeout!\n");
            break;
        }
    }

    close(rfd);
    close(wfd);

    return 0;
}