#include <Cfun.h>

/**
 * 利用链表实现一个队列
 * 创建共享资源，并初始化条件变量和互斥锁
 * 基于互斥锁和条件变量，根据共享资源的多少情况，生产者和消费者广播提醒其他线程
 * 实现队列的生产者消费者模型
 */

typedef struct Node
{
    int data_t;
    struct Node *next_t;
} mNode;

typedef struct Queue
{
    struct Node *front_t;
    struct Node *rear_t;
    int size_t;
} mQueue;

int pushQueue(mQueue *queue, int data)
{
    // mNode *node = (mNode *)malloc(sizeof(mNode));
    mNode *node = (mNode *)calloc(1, sizeof(mNode));
    node->data_t = data;
    if (queue->size_t == 0)
    {
        queue->front_t = node;
        queue->rear_t = node;
    }
    else
    {
        queue->rear_t->next_t = node;
        queue->rear_t = node;
    }
    ++queue->size_t;

    return 0;
}

int popQueue(mQueue *queue)
{
    if (queue->size_t == 0)
    {
        return -1;
    }

    mNode *del = queue->front_t;
    queue->front_t = queue->front_t->next_t;
    if (queue->size_t == 1)
    {
        queue->rear_t = NULL;
    }
    free(del);
    --queue->size_t;

    return 0;
}

int visitQueue(mQueue *queue)
{
    if (queue->size_t == 0)
    {
        printf("Queue is empty\n");
        return 0;
    }
    mNode *node = queue->front_t;
    while (node != NULL)
    {
        printf("%d ", node->data_t);
        node = node->next_t;
    }
    printf("\n");
    return 0;
}

// 单元测试函数
void test()
{
    mQueue myQueue;
    memset(&myQueue, 0, sizeof(myQueue));
    for (int i = 0; i < 10; ++i)
    {
        int data = rand() % 1000;
        pushQueue(&myQueue, data);
        printf("data = %d\n", data);
        visitQueue(&myQueue);
    }
    printf("-----------------------\n");
    for (int i = 0; i < 10; ++i)
    {
        int front = myQueue.front_t->data_t;
        popQueue(&myQueue);
        printf("front = %d\n", front);
        visitQueue(&myQueue);
    }
    for (int i = 0; i < 10; ++i)
    {
        int data = rand() % 1000;
        pushQueue(&myQueue, data);
        printf("data = %d\n", data);
        visitQueue(&myQueue);
    }
}

typedef struct share_Res_t
{
    mQueue queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shared;

void *Producer_Thread(void *arg)
{
    shared *share = (shared *)arg;
    while (1)
    {
        pthread_mutex_lock(&share->mutex);
        while (share->queue.size_t >= 10)
        {
            pthread_cond_wait(&share->cond, &share->mutex);
        }
        int data = rand() % 1000;
        pushQueue(&share->queue, data);
        visitQueue(&share->queue);

        pthread_cond_broadcast(&share->cond);
        pthread_mutex_unlock(&share->mutex);
        sleep(3);
    }

    pthread_exit(NULL);
}

void *Consumer_Thread(void *arg)
{
    shared *share = (shared *)arg;
    while (1)
    {

        pthread_mutex_lock(&share->mutex);
        while (share->queue.size_t <= 1)
        {
            pthread_cond_wait(&share->cond, &share->mutex);
        }
        popQueue(&share->queue);
        visitQueue(&share->queue);

        pthread_cond_broadcast(&share->cond);
        pthread_mutex_unlock(&share->mutex);
        sleep(1);
    }

    pthread_exit(NULL);
}

int main()
{
    shared share;
    pthread_mutex_init(&share.mutex, NULL);
    pthread_cond_init(&share.cond, NULL);
    memset(&share.queue, 0, sizeof(share.queue));
    for (int i = 0; i < 8; ++i)
    {
        int data = rand() % 1000;
        pushQueue(&share.queue, data);
        printf("data = %d\n", data);
        visitQueue(&share.queue);
    }
    printf("-----------------------\n");

    pthread_t Producer1, Producer2, Producer3, Consumer1, Consumer2, Consumer3;
    pthread_create(&Producer1, NULL, Producer_Thread, &share);
    pthread_create(&Producer2, NULL, Producer_Thread, &share);
    pthread_create(&Producer3, NULL, Producer_Thread, &share);
    pthread_create(&Consumer1, NULL, Consumer_Thread, &share);
    pthread_create(&Consumer2, NULL, Consumer_Thread, &share);
    // pthread_create(&Consumer3, NULL, Consumer_Thread, &share);
    pthread_join(Producer1, NULL);
    pthread_join(Producer2, NULL);
    pthread_join(Producer3, NULL);
    pthread_join(Consumer1, NULL);
    pthread_join(Consumer2, NULL);
    // pthread_join(Consumer3, NULL);

    return 0;
}