#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * 匿名管道的系统调用
 *
 * int pipe(int pipefd[2])
 *
 * int pipefd[2]数组，保存函数返回的两个文件描述符：fd[0]指向读端，fd[1]指向写端
 * 
 * 只能用于具有血缘关系的进程之间，进程之间数据的单向传递
 * 可以使用文件IO中的read和write进行读写
 * 内核空间中的内存段
 * 管道大小固定，管道写满数据时，将会阻塞写入
 */

/* 创建两个管道，全部定义为单工的管道，
父进程向管道1中读取数据，子进程向管道1中写入数据，
父进程从管道2中写入数据，子进程从管道2中读取数据
父进程写hello，子进程写world，读取到数据后直接打印出来*/
int main(int argc, char *argv[])
{
    // fds1 read -> write; fds2 write -> read
    int fds1[2];
    int fds2[2];
    pipe(fds1);
    pipe(fds2);

    if (fork())
    {
        close(fds1[1]);
        close(fds2[0]);

        char buf[1024] = {0};
        read(fds1[0], buf, 5);
        printf("Child read: %s\n", buf);

        write(fds2[1], "world", 5);

        close(fds1[0]);
        close(fds2[1]);

        wait(NULL);
    }
    else
    {
        close(fds1[0]);
        close(fds2[1]);

        write(fds1[1], "hello", 5);
        
        char buf[1024] = {0};
        read(fds2[0], buf, 5);
        printf("Parent read: %s\n", buf);

        close(fds1[1]);
        close(fds2[0]);
    }

    return 0;
}