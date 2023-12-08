#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS 5
#define NUM_ROUNDS 10
//確保只有一個人在吃
int state = 0;

pthread_mutex_t chopsticks[NUM_CHOPSTICKS];
//pthread_cound_t condition[NUM_PHILOSOPHERS];

void *philosopher(void *arg){
    int philosopher_id = *(int *)arg;
    int left_chopstick = philosopher_id;
    int right_chopstick = (philosopher_id + 1) % NUM_CHOPSTICKS;
    //修改部分
    for(int i = 0; i < NUM_ROUNDS; i++){
        // pick up chopsticks
        printf("Philosopher %d is try to pick up chopsticks\n", philosopher_id);
        if (pthread_mutex_trylock(&chopsticks[left_chopstick])==0){
           if(pthread_mutex_trylock(&chopsticks[right_chopstick])==0){
                if(state == 0){
                    //eating，只有兩支筷子都拿到才能吃
                    printf("Philosopher %d is eating\n", philosopher_id);
                    //只有一個人在吃
                    state = 1;
                    usleep(rand() % 1000000);
                    // put down chopsticks
                    pthread_mutex_unlock(&chopsticks[left_chopstick]);
                    pthread_mutex_unlock(&chopsticks[right_chopstick]);
                    state = 0;
                }else{
                    //已經有人在吃了，放下右邊筷子等待可以吃，可以拿著左邊筷子
                    printf("Philosopher %d is waiting to pick up right chopstick\n", philosopher_id);
                    pthread_mutex_unlock(&chopsticks[right_chopstick]);
                }
                
           }else{
                // failed to pick up right chopstick
                pthread_mutex_unlock(&chopsticks[left_chopstick]);
                printf("Philosopher %d is failed to pick up right chopstick\n", philosopher_id);
                usleep(rand() % 1000000);
           }
        }else{
            // failed to pick up left chopstick
            usleep(rand() % 1000000);
            printf("Philosopher %d is failed to pick up left chopstick\n", philosopher_id);
        }
        
    }

    /* sample code
    while (1)
    {
        // thinking
        printf("Philosopher %d is thinking\n", philosopher_id);
        usleep(rand() % 1000000);

        // pick up chopsticks
        printf("Philosopher %d is hungry\n", philosopher_id);
        while(pthread_mutex_trylock(&chopsticks[left_chopstick]) != 0){
            // failed to pick up right chopstick
            pthread_mutex_unlock(&chopsticks[left_chopstick]);
            usleep(rand() % 1000000);
            pthread_mutex_lock(&chopsticks[left_chopstick]);
        }

        //eating
        printf("Philosopher %d is eating\n", philosopher_id);
        usleep(rand() % 1000000);

        // put down chopsticks
        pthread_mutex_unlock(&chopsticks[left_chopstick]);
        pthread_mutex_unlock(&chopsticks[right_chopstick]);

        //Notify neighbors
        pthread_cond_signal(&condition[left_chopstick]);
        pthread_cond_signal(&condition[right_chopstick]);
    }
    */
}

int main(){
    pthread_t tid[NUM_PHILOSOPHERS];
    int phill[NUM_PHILOSOPHERS];

    //init
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        pthread_mutex_init(&chopsticks[i], NULL);
        //pthread_cond_init(&condition[i], NULL);
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

    //cleanup
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        pthread_mutex_destroy(&chopsticks[i]);
       // pthread_cond_destroy(&condition[i]);
    }

    return 0;
}