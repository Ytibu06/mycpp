#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


/* 自定义传输协议 */
typedef struct train_s{
    int length;
    char data[1000];
}trains;

int main(int argc, char* argv[])
{
    //./readFileInfo fifo dirpath

    if(argc != 3){
        printf("argc error!\n");
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd < 0){
        perror("open");
        exit(1);
    }

    trains train;
    char filename[1024] = {0};

    /* 读取文件名，创建文件，并打开文件*/
    int read_ret = read(fd, &train.length, sizeof(train.length));
    read_ret = read(fd, train.data, train.length);
    memcpy(filename, train.data, train.length);

    char dir[1024] = {0};
    strcpy(dir, argv[2]);
    mkdir(dir, 0664);

    char path[1048] = {0};
    int n = snprintf(path, sizeof(path), "%s/%s", dir, filename);
    // 使用snprintf防止缓冲区溢出
    if(n >= sizeof(path)) {
        printf("Path too long!\n");
        exit(1);
    }

    int file = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0664);
    if(file < 0){
        perror("open");
        exit(1);
    }

    
    /* 读取文件内容，写入文件中*/
    read_ret = read(fd, &train.length, sizeof(train.length));  // 从源文件fd读取长度
    read_ret = read(fd, train.data, train.length);            // 从源文件fd读取内容
    write(file, train.data, train.length);

    close(fd);
    close(file);

    return 0;
}