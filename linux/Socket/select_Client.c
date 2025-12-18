#include <Cfun.h>


void Client(const char *ip, int port)
{

}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s ip port\n", basename(argv[0]));
        exit(1);
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    Client(ip, port);
    
    return 0;
}