#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// 定義序列的大小
#define SIZE 20
#define NUMBER_OF_THREADS 6

void *sorter(void *params); // 排序
void *merger(void *params); // 合併

int list[SIZE];
int result[SIZE];
//共享的全局變數
int sharedResource = 0;
int sharedVariable = 0;

//互斥鎖
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

//執行續函數
void *threadFunction(void *arg)
{
    int threadID = *(int *)arg;

    for (int i = 0; i < 5; ++i)
    {
        //上鎖
        pthread_mutex_lock(&mutex);

        //進行共享資源的操作
        sharedResource++;
        printf("Thread %d: shared Resource = %d\n", threadID, sharedResource);

        //解鎖
        pthread_mutex_unlock(&mutex);

        //使執行續睡眠一段時間，模擬其他工作
        usleep(100000);
    }
    return NULL;
}
//trylock
void *incrementCounter(void *arg)
{
    int lockfor;
    for (lockfor = 0; lockfor < 100000; ++lockfor)
    {
        //嘗試鎖定互斥鎖
        if(pthread_mutex_trylock(&mutex) == 0)
        {
            //臨界區:修改共享資源
            sharedVariable++;
            //解鎖
            pthread_mutex_unlock(&mutex);
        }else{
            //未能鎖定互斥鎖，等待一小段時間
            usleep(1000);//等待一毫秒(需要include <unistd.h>)
            //這裡也可以做其他事情
        }
    }
    
    return NULL;
}

int main(int argc, const char *argv[]) {
    pthread_t thread1,thread2;

    int threadID1 = 1;
    int threadID2 = 2;

    //創建兩個執行續
    pthread_create(&thread1, NULL, incrementCounter, &threadID1);
    pthread_create(&thread2, NULL, incrementCounter, &threadID2);

    //等待執行續結束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    //銷毀互斥鎖
    pthread_mutex_destroy(&mutex);

    printf("sharedVariable = %d\n", sharedVariable);

    return 0;
}


