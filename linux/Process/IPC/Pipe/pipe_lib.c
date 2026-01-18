#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * 匿名管道的库函数
 *
 * FILE *popen(const char *command, const char *type)
 * int pclose(FILE *stream)
 *
 * 通过popen函数执行命令来创建一个进程，并指定自己对管道的操作方式(数据的流向)，并返回一个FILE指针。
 * popen函数返回一个FILE指针，该指针指向一个FILE结构体，该结构体描述了管道，
 * 有血缘关系的进程之间进行数据的单向传输。
 */

void open_r()
{
    // ./printf.out
    FILE *fp = popen("./printf.out", "r");
    //创建子进程并指定输入输出(数据流向)会重定向标准输入和标准输出到管道中

    char buf[1024] = {0};
    fread(buf, 1, sizeof(buf), fp);
    printf("buf = %s", buf);
    pclose(fp);
}

void open_w()
{
    //./ scanf.out 
    FILE *fp1 = popen("./scanf.out", "w"); // 开启无名管道

    fwrite("5 6", 1, 3, fp1); // 此文件流可以进行正常的读写
    pclose(fp1);              // 关闭管道
}

/*标准管道*/
int main(int argc, char *argv[])
{
    open_r();
    open_w();
    return 0;
}