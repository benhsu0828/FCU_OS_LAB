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
int sharedVariable = 0;
int failTryLock = 0;

//互斥鎖
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

typedef struct
{
    int from_index;
    int to_index;
} parameters;

int main(int argc, const char *argv[]) {
    int i;
    int h;
    int length;
    int arr[length];

    // 輸入 sort 大小，並隨機產生 sort 大小的亂數
    printf("請輸入排序大小: ");
    scanf("%d", &length);
    for (h = 0; h < length; h++) {
        list[h] = rand() % 100;
        printf("%d ", list[h]);
    }
    printf("\n");
    pthread_t workers[NUMBER_OF_THREADS];

    // 使用 clock() #clock_t clock(void);
    // startime, endtime 用來計算合併排序的時間
    clock_t startime, endtime;
    startime = clock();

    // 建立第一個排序執行緒
    parameters *data = (parameters *)malloc(sizeof(parameters));
    data->from_index = 0;
    data->to_index = (SIZE / 5) - 1;
    printf("建立第一個thread\n");
    pthread_create(&workers[0], 0, sorter, data);

    // 建立第二個排序執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = (SIZE / 5);
    data->to_index = (SIZE / 5)*2 - 1;
    printf("建立第二個thread\n");
    pthread_create(&workers[1], 0, sorter, data);

    // 建立第三個排序執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = (SIZE / 5)*2;
    data->to_index = (SIZE / 5)*3 - 1;
    printf("建立第三個thread\n");
    pthread_create(&workers[2], 0, sorter, data);

    // 建立第四個排序執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = (SIZE / 5)*3;
    data->to_index = (SIZE / 5)*4 - 1;
    printf("建立第四個thread\n");
    pthread_create(&workers[3], 0, sorter, data);

    // 建立第五個排序執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = (SIZE / 5)*4;
    data->to_index = SIZE - 1;
    printf("建立第五個thread\n");
    pthread_create(&workers[4], 0, sorter, data);

    // 等待兩個排序執行緒完成
    for (i = 0; i < NUMBER_OF_THREADS - 1; i++) {
        pthread_join(workers[i], NULL);
    }

    // 建立合併執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = 0;
    data->to_index = (SIZE / 2);
    pthread_create(&workers[5], 0, merger, data);

    // 等待合併執行緒完成
    pthread_join(workers[5], NULL);

    endtime = clock();

    // CLOCKS_PER_SEC 單位微秒
    printf("\n");
    printf("Time taken: %f\n", (endtime - startime) / (double)CLOCKS_PER_SEC);

    //銷毀互斥鎖
    pthread_mutex_destroy(&mutex);
    //顯示執行成功多少次trylock
    printf("執行成功%d次trylock %d次trylock失敗\n", sharedVariable,failTryLock);
    return 0;
}

void *sorter(void *params) {

    int lockfor;
    for (lockfor = 0; lockfor < 100000; ++lockfor)
    {
        //嘗試鎖定互斥鎖
        if(pthread_mutex_trylock(&mutex) == 0)
        {
            //成功鎖定互斥鎖
            //臨界區:修改共享資源
            sharedVariable++;
            printf("成功對thread %d鎖定互斥鎖，進行sort\n", sharedVariable);
            // 包含 from_index, to_index
            parameters *p = (parameters *)params;

            // 執行第一個 thread 排序
            int begin = p->from_index;
            int end = p->to_index + 1;
            int z;
            for (z = begin; z < end; z++) {
                printf("The array recieved is: %d\n", list[z]);
            }

            printf("\n");

            int i, j, t, k;

            // 泡沫排序法，排序從小到大
            for (i = begin; i < end; i++) {
                for (j = begin; j < end - 1; j++) {
                    if (list[j] > list[j + 1]) {
                        t = list[j];
                        list[j] = list[j + 1];
                        list[j + 1] = t;
                    }
                }
            }

            for (k = begin; k < end; k++) {
                printf("The sorted array: %d\n", list[k]);
            }

            int x;
            for (x = begin; x < end; x++) {
                result[x] = list[x];
            }

             //解鎖
            pthread_mutex_unlock(&mutex);
            printf("成功對thread %d解鎖互斥鎖\n\n", sharedVariable);
            pthread_exit(NULL);
            //跳出for迴圈
            break;
        }else{
            //未能鎖定互斥鎖，等待一小段時間
            usleep(1000);//等待一毫秒(需要include <unistd.h>)
            //這裡也可以做其他事情
            failTryLock++;
            printf("未能鎖定互斥鎖\n");
        }
    }
    
}

void *merger(void *params) {
    int lockfor;
    for (lockfor = 0; lockfor < 100000; ++lockfor)
    {
        //嘗試鎖定互斥鎖
        if(pthread_mutex_trylock(&mutex) == 0)
        {
            //成功鎖定互斥鎖
            printf("成功鎖定互斥鎖，進行merge\n");
            //臨界區:修改共享資源
            sharedVariable++;
            // 包含 from_index, to_index
            parameters *p = (parameters *)params;

            // 執行第二個 thread 合併
            int begin = p->from_index;
            int end = p->to_index+1;
            int i, j, t;

            // 泡沫排序法，排序從小到大
            for (i = begin; i < end; i++) {
                for (j = begin; j < end - 1; j++) {
                    if (result[j] > result[j + 1]) {
                        t = result[j];
                        result[j] = result[j + 1];
                        result[j + 1] = t;
                    }
                }
            }
            printf("\n");
            int d;
            // 顯示合併後的 sort
            for (d = 0; d < SIZE; d++) {
                printf("The merged array: %d\n", result[d]);
            }
            //解鎖
            pthread_mutex_unlock(&mutex);
            printf("成功解鎖互斥鎖\n\n");
            pthread_exit(NULL);
            //跳出for迴圈
            break;
        }else{
            //未能鎖定互斥鎖，等待一小段時間
            usleep(1000);//等待一毫秒(需要include <unistd.h>)
            //這裡也可以做其他事情
            failTryLock++;
            printf("未能鎖定互斥鎖\n");
        }
    }
    
}