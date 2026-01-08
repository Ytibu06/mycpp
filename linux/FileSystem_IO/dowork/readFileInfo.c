#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 自定义传输协议 */
typedef struct train_s{
    int length;
    char data[1000];
}trains;


int main(int argc, char* argv[])
{
    //./readFileInfo fifo filename
    if(argc !=3){
        printf("argc error!\n");
        exit(1);
    }

    int fd = open(argv[1], O_WRONLY);
    if(fd < 0){
        perror("open");
        exit(1);
    }

    int file = open(argv[2], O_RDONLY);
    if(file < 0){
        perror("open");
        exit(1);
    }

    char buf[1024] = {0};
    trains train;

    /* 获取文件名及其长度，依次将文件名的内容和长度写入fd */
    train.length = strlen(argv[2]);
    memcpy(train.data, argv[2], train.length);
    int write_ret = write(fd, &train.length, sizeof(train.length));
    write_ret = write(fd, train.data, train.length);

    /* 获取文件内容，依次将文件内容及其长度写入fd*/
    train.length = read(file, train.data, sizeof(train.data));
    write_ret = write(fd, &train.length, sizeof(train.length));  // 修正：使用sizeof(train.length)而不是train.length
    write_ret = write(fd, train.data, train.length);

    close(fd);
    close(file);

    return 0;

}