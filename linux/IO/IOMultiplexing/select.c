#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

/*
    int select (
        int __nfds, //需要监视的文件描述符集合中最大文件描述符值加 1
        fd_set * __readfds, //指向一个 fd_set 类型的指针，用于指定你希望监视其可读性的文件描述符集合
        fd_set * __writefds,    //指向一个 fd_set，用于指定你希望监视其可写性的文件描述符集合
        fd_set * __exceptfds,   //指向一个 fd_set，用于监视文件描述符上的异常条件（如带外数据 OOB、连接断开等）
        struct timeval * __timeout  //指定 select() 的最长阻塞等待时间。
    )

    struct timeval{
        __time64_t tv_sec;		//Seconds.
        __suseconds64_t tv_usec;  // Microseconds.
    };
*/

int main(int argc, const char *argv[])
{
    if (mkfifo("/tmp/fifo", 0666) == -1)
    {
        perror("mkfifo");
        // 如果 FIFO 已存在，mkfifo 会失败（errno = EEXIST）
        // 可以选择忽略这个错误
    }
    int rfd = open("/tmp/fifo", O_RDWR);
    int wfd = open("/tmp/fifo", O_WRONLY);

    fd_set rdset;
    fd_set wrset;
    int cnt = 0;
    char buf[4096] = {0};
    int maxfd = (rfd > wfd) ? rfd : wfd; 
    while (1)
    {
        FD_ZERO(&rdset); // 清空
        FD_ZERO(&wrset);
        FD_SET(rfd, &rdset);
        FD_SET(wfd, &wrset);
        struct timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        int ret = select(maxfd + 1, &rdset, NULL, NULL, &timeout);

        if(FD_ISSET(rfd,&rdset)){
            printf("read ready, cnt = %d, ret = %d\n", cnt++, ret);
            read(rfd,buf,sizeof(buf)/2);
        }   //如果FIFO中没有数据可读，可能引起阻塞
        if(FD_ISSET(wfd,&wrset)){
            printf("write ready, cnt = %d, ret = %d\n", cnt++, ret);
            write(wfd,buf,sizeof(buf));
        }   //如果FIFO缓冲区满且没有读端读数据时，写操作阻塞

        sleep(1);
    }

    return 0;
}