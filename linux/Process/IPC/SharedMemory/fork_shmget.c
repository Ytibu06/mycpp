#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SUM_NUM 1000000

int main(int argc, char *argv[])
{
    int shmid = shmget(0x1238, 1024, IPC_CREAT | 0666);
    int *shmaddr = shmat(shmid, NULL, 0);
    *shmaddr = 100;

    int num = SUM_NUM;
    if (fork())
    {
        for (int i = 0; i < num; ++i)
        {
            ++*shmaddr;
        }
        wait(NULL);
        printf("shmaddr = %d\n", *shmaddr);
    }
    else
    {
        for (int i = 0; i < num; ++i)
        {
            ++*shmaddr;
        }
    }

    return 0;
}