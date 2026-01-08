#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    int wfd = open("/tmp/fifo1", O_WRONLY);
    if (wfd < 0)
    {
        perror("open");
        exit(1);
    }
    int rfd = open("/tmp/fifo2", O_RDONLY);
    if (rfd < 0)
    {
        perror("open");
        exit(1);
    }

    int maxfd = rfd > STDIN_FILENO ? rfd : STDIN_FILENO;
    fd_set rfds;
    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(rfd, &rfds);
        FD_SET(STDIN_FILENO, &rfds);
        int ret = select(maxfd + 1, &rfds, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &rfds))
        {
            char buf[1024] = {0};
            int len = read(STDIN_FILENO, buf, sizeof(buf));
            if (len <= 0)
            {
                perror("read");
                break;
            }
            write(wfd, buf, len);
        }

        if (FD_ISSET(rfd, &rfds))
        {
            char buf[1024] = {0};
            int len = read(rfd, buf, sizeof(buf));
            if (len <= 0)
            {
                printf("The Client Connect is closed!\n");
                break;
            }
            printf("recevive Server: %s", buf);
        }
    }

    close(rfd);
    close(wfd);

    return 0;
}