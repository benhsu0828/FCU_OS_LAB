#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 定義序列的大小
#define SIZE 20
#define NUMBER_OF_THREADS 6

void *sorter(void *params); // 排序
void *merger(void *params); // 合併

int list[SIZE];
int result[SIZE];

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
    pthread_create(&workers[0], 0, sorter, data);

    // 建立第二個排序執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = (SIZE / 5);
    data->to_index = (SIZE / 5)*2 - 1;
    pthread_create(&workers[1], 0, sorter, data);

    // 建立第三個排序執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = (SIZE / 5)*2;
    data->to_index = (SIZE / 5)*3 - 1;
    pthread_create(&workers[2], 0, sorter, data);

    // 建立第四個排序執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = (SIZE / 5)*3;
    data->to_index = (SIZE / 5)*4 - 1;
    pthread_create(&workers[3], 0, sorter, data);

    // 建立第五個排序執行緒
    data = (parameters *)malloc(sizeof(parameters));
    data->from_index = (SIZE / 5)*4;
    data->to_index = SIZE - 1;
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

    return 0;
}

void *sorter(void *params) {
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

    printf("\n");
    pthread_exit(NULL);
}

void *merger(void *params) {
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
    pthread_exit(NULL);
}