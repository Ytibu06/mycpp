#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>


/*
 * #include <sys/types.h>
 * #include <sys/stat.h>
 *
 * 有名管道
 * int mkfifo(const char *pathname, mode_t mode);
 * pathname: 文件名; mode: 权限
 * 
 * 通过创建一个类似于文件的管道，借助此文件来完成进程间的通信，任何进程之间都可以借此进行通信
 *
 * 创建成功返回0, 失败返回-1
 * 只读打开会阻塞，只有当其他进程只写打开同意管道才返回，打开操作以读写或只写打开不会阻塞
 */

/*有名管道*/
int main(int argc, char *argv[])
{

    // 创建命名管道
    if (mkfifo("/tmp/myfifo", 0664) < 0)
    {
        perror("mkfifo");
    }

    // 打开命名管道
    //int fd = open("/tmp/myfifo", O_WRONLY); // 或 O_WRONLY


    if(fork()){
        int fd = open("/tmp/myfifo", O_WRONLY);
        const char *msg = "hello world";
        write(fd, msg, strlen(msg));
        printf("Parent write: hello world\n");
        close(fd);

        wait(NULL);
    }else{
        int fd = open("/tmp/myfifo", O_RDONLY);
        char buf[1024];
        int len = read(fd, buf, sizeof(buf)-1);
        buf[len] = '\0';

        printf("Child read: %s\n", buf);

        close(fd);
    }

    return 0;
}