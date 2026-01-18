/**
 * 进程间通信方式：管道、共享内存、信号、内存映射、信号量、消息队列、套接字
 *      管道：有名管道mkfifo、匿名管道pipe系统调用和popen标准库函数
 *      共享内存：shmget、shmat、shmdt、shmctl，速度快
 *      信号：signal，修改信号处理函数
 *      套接字：socket，即网络编程
 */