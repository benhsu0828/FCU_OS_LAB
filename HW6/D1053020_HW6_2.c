#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 7
#define NUM_CHOPSTICKS 6
#define NUM_ROUNDS 5
int IsInWaiter(int);
void AskWaiter(int);

pthread_mutex_t chopsticks[NUM_CHOPSTICKS];

int waiter[NUM_PHILOSOPHERS];//新增服務生
int check = 0;//名單順序
int eating = 0;//正在吃飯的人數

void AskWaiter(int philosopher_id){
    //如果不再等待名單中，加入等待名單
    if (IsInWaiter(philosopher_id) == 0){
        waiter[check] = philosopher_id;
        check = (check+1)%NUM_PHILOSOPHERS;
    }else{
        //如果正在吃飯的人數小於4，且在等待名單中有人，開始吃飯
        if (eating <= (NUM_CHOPSTICKS/2)-1&&waiter[eating]!=-1){
            //可以吃飯，吃飯人數+1，創造一個區域變數避免數字被改到
            int nowEating = eating;
            int p = waiter[nowEating];
            //先將等待名單中的人往前移一位
            for(int i = nowEating; i<check; i++){
                waiter[i] = waiter[i+1];
            }
            for(int i = check-1; i<NUM_PHILOSOPHERS; i++){
                waiter[i] = -1;
            }
            check = (check-1)%NUM_PHILOSOPHERS;
            eating++;
            printf("Philosopher %d is eating\n", p);
            pthread_mutex_trylock(&chopsticks[nowEating*2]);
            pthread_mutex_trylock(&chopsticks[(nowEating*2 + 1) % NUM_CHOPSTICKS]);
            usleep(rand() % 1000000);
            //吃飯完畢，吃飯人數-1，從等待名單中移除
            pthread_mutex_unlock(&chopsticks[nowEating*2]);
            pthread_mutex_unlock(&chopsticks[(nowEating*2 + 1) % NUM_CHOPSTICKS]);
            printf("Philosopher %d is finish\n", p);
            eating--;
        }else if (eating > NUM_CHOPSTICKS/2-1){
            //超過三人，不可以吃飯，繼續等待
            printf("Philosopher %d is waiting\n", philosopher_id);
        }
    }
}

int IsInWaiter(int philosopher_id){
    //檢查是否在等待名單中
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        if(waiter[i] == philosopher_id){
            return 1;
        }
    }
    return 0;
}

void *philosopher(void *arg){
    int philosopher_id = *(int *)arg;

    for(int i = 0; i < NUM_ROUNDS; i++){
       AskWaiter(philosopher_id);

    }
}

int main(){
    pthread_t tid[NUM_PHILOSOPHERS];
    int phill[NUM_PHILOSOPHERS];

    //init
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        pthread_mutex_init(&chopsticks[i], NULL);
        waiter[NUM_PHILOSOPHERS] = -1;
    }

    //create threads
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        phill[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &phill[i]);
    }

    //wait for all threads to finish
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        pthread_join(tid[i], NULL);
    }

    //檢查是否有人還在等待
    while (1)
    {
        if (eating <= NUM_CHOPSTICKS/2-1&&waiter[eating]!=-1){
            //可以吃飯，吃飯人數+1，創造一個區域變數避免數字被改到
            int nowEating = eating;
            int p = waiter[nowEating];
            //先將等待名單中的人往前移一位
            for(int i = nowEating; i<check; i++){
                waiter[i] = waiter[i+1];
            }
            for(int i = check-1; i<NUM_PHILOSOPHERS; i++){
                waiter[i] = -1;
            }
            check = (check-1)%NUM_PHILOSOPHERS;
            eating++;
            printf("Philosopher %d is eating\n", p);
            pthread_mutex_trylock(&chopsticks[nowEating*2]);
            pthread_mutex_trylock(&chopsticks[(nowEating*2 + 1) % NUM_CHOPSTICKS]);
            usleep(rand() % 1000000);
            //吃飯完畢，吃飯人數-1，從等待名單中移除
            pthread_mutex_unlock(&chopsticks[nowEating*2]);
            pthread_mutex_unlock(&chopsticks[(nowEating*2 + 1) % NUM_CHOPSTICKS]);
            printf("Philosopher %d is finish\n", p);
            eating--;
        }else if(waiter[0] ==-1){
            //沒有人在等待，結束
            break;
        }
    }
    

    //cleanup
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        pthread_mutex_destroy(&chopsticks[i]);
    }

    return 0;
}