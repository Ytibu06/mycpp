#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * 系统调用头文件
 * #include <sys/shm.h>
 *
 * int shmget(key_t key, size_t size, int shmflg);  // 创建物理共享内存
 * void *shmat(int shmid, const void *shmaddr, int shmflg);  // 虚拟内存映射共享内存
 * int shmdt(const void *shmaddr);  // 删除虚拟内存映射
 *
 * key 是共享内存的标识符，0/IPC_PRIVATE 表示创建一个私有的共享内存
 * size 是共享内存的大小
 * shmflg 是共享内存的属性，创建标志|权限标志：IPC_CREAT | 0666
 */

int main(int argc, char *argv[])
{
    int shmid = shmget(0x1234, 1024, IPC_CREAT | 0666); // 创建共享内存

    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }

    printf("shmid = %d\n", shmid);

    char *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr < 0)
    {
        perror("shmat");
        exit(1);
    }

    memcpy(shmaddr, "hello world", 11);
    printf("shmaddr = %s\n", shmaddr);

    shmdt(shmaddr);

    return 0;
}