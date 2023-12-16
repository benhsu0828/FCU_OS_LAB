#include <stdio.h>

struct Process
{
    int id;
    int arrival_time;
    int burst_time;
};
struct WaitingQueue
{
    int id;
    int arrival_time;
    int burst_time;
    int lastBurstTime;
};


//平均周轉時間和平均等待時間
void calculateAvgTimes(struct Process processes[],int n, int maxTimeQuantum){
    int waitTime[n], turnaroundTime[n];
    int timeQuantum = 1;
    float shortestTurnaroundTime = 0, shortestWaitTime = 0;
    float avgTurnaroundTime = 0, avgWaitTime = 0; 
    int shortestTimeQuantum = 0;
    int currentTime = 0;
    int completed = 0;
    struct WaitingQueue waitingQueue[n];
    int changeFlag = 0;//檢查是否有執行過 0:沒有 1:有
   //init
   for (int i = 0; i < n; i++)
   {
    waitTime[i] = 0;
    turnaroundTime[i] = 0;
    waitingQueue[i].id = processes[i].id;
    waitingQueue[i].arrival_time = processes[i].arrival_time;
    waitingQueue[i].burst_time = processes[i].burst_time;
    waitingQueue[i].lastBurstTime = processes[i].burst_time;
   }
   //sort waitingQueue by arrival time
    for(int i = 0; i < n; i++){
         for(int j = i+1; j < n; j++){
              if(waitingQueue[i].arrival_time > waitingQueue[j].arrival_time){
                struct WaitingQueue temp = waitingQueue[i];
                waitingQueue[i] = waitingQueue[j];
                waitingQueue[j] = temp;
              }
         }
    }

    //RR排班，窮舉出最短的time quantum
    for(timeQuantum = 1;timeQuantum <= maxTimeQuantum;timeQuantum++){
        //將第一個process的到達時間設為currentTime
        currentTime = waitingQueue[0].arrival_time;
        while(completed < n){
            for(int i = 0; i < n; i++){
                // //印出目前資料
                // printf("TQ: %d\nCurrent Time: %d\nID: %d\narrival time: %d\nlastBurstTime: %d\n",timeQuantum,currentTime,waitingQueue[i].id,waitingQueue[i].arrival_time,waitingQueue[i].lastBurstTime);
                // for(int j = 0; j < n; j++){
                //     printf("waitingQueue: %d ",waitingQueue[j].id);
                // }
                // printf("\n-------------------------------------------\n");
                if(waitingQueue[i].lastBurstTime == 0){//如果已經完成，則跳過
                    continue;
                }
                if(waitingQueue[i].arrival_time > currentTime){//如果還沒到這個process的執行時間，則跳過
                    continue;
                }
                changeFlag = 1;
                if(waitingQueue[i].lastBurstTime > timeQuantum){
                    currentTime += timeQuantum;
                    waitingQueue[i].lastBurstTime -= timeQuantum;
                    //未完成，將其放到最後，並且將其他的往前移
                    struct WaitingQueue temp = waitingQueue[i];
                    for(int j = i; j < n-1; j++){
                        waitingQueue[j] = waitingQueue[j+1];
                    }
                    i--;
                    waitingQueue[n-1] = temp;
                }else{
                    currentTime += waitingQueue[i].lastBurstTime;
                    turnaroundTime[waitingQueue[i].id-1] = currentTime - waitingQueue[i].arrival_time;
                    waitTime[waitingQueue[i].id-1] = turnaroundTime[waitingQueue[i].id-1] - waitingQueue[i].burst_time;
                    waitingQueue[i].lastBurstTime = 0;
                    completed++;
                }
                //反覆檢查waiting queue，如果還沒到此process的執行時間，保持在waitingQueue的第最後一個
                for(int k = 0; k < n; k++){
                    if(waitingQueue[k].arrival_time > currentTime){
                        struct WaitingQueue temp = waitingQueue[k];
                        for(int j = k; j < n-1; j++){
                            waitingQueue[j] = waitingQueue[j+1];
                        }
                        waitingQueue[n-1] = temp;
                    }
                }
            }
            if(changeFlag == 0){//如果沒有執行過，則時間往後移
                currentTime++;
            }
            changeFlag = 0;
        }
         //計算平均等待時間和平均周轉時間
        for (int i = 0; i < n; i++)
        {
            avgTurnaroundTime += turnaroundTime[i];
            avgWaitTime += waitTime[i];
        }
        avgTurnaroundTime /= n;
        avgWaitTime /= n;
        if(timeQuantum == 1){//第一次執行
            shortestTurnaroundTime = avgTurnaroundTime;
            shortestWaitTime = avgWaitTime;
            shortestTimeQuantum = timeQuantum;
        }
        if(shortestWaitTime >= avgWaitTime){//新增等於，減少context switch的時間
            shortestTurnaroundTime = avgTurnaroundTime;
            shortestWaitTime = avgWaitTime;
            shortestTimeQuantum = timeQuantum;
        }
        printf("-------------------------------------------\n");
        printf("Time Quantum: %d\n",timeQuantum);
        printf("Average Turnaround Time = %.2f\n", avgTurnaroundTime);
        printf("Average Waiting Time = %.2f\n", avgWaitTime);
        //init
        avgTurnaroundTime = 0;
        avgWaitTime = 0;
        currentTime = 0;
        completed = 0;
        for (int i = 0; i < n; i++)
        {
            waitTime[i] = 0;
            turnaroundTime[i] = 0;
            waitingQueue[i].id = processes[i].id;
            waitingQueue[i].arrival_time = processes[i].arrival_time;
            waitingQueue[i].burst_time = processes[i].burst_time;
            waitingQueue[i].lastBurstTime = processes[i].burst_time;
        }
        //sort waitingQueue by arrival time
        for(int i = 0; i < n; i++){
            for(int j = i+1; j < n; j++){
                if(waitingQueue[i].arrival_time > waitingQueue[j].arrival_time){
                    struct WaitingQueue temp = waitingQueue[i];
                    waitingQueue[i] = waitingQueue[j];
                    waitingQueue[j] = temp;
                }
            }
        }
    }
   
    //印出結果
    printf("\nProcess Information:\n");
    printf("-------------------------------------------\n");
    printf("| Process | Arrival Time | Burst Time |\n");
    printf("-------------------------------------------\n");
    for(int i = 0; i < n; i++){
        printf("| P%-8d | %-13d | %-11d |\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time);
    }
    printf("-------------------------------------------\n");
    printf("Time Quantum: %d\n",shortestTimeQuantum);
    printf("Average Turnaround Time = %.2f\n", shortestTurnaroundTime);
    printf("Average Waiting Time = %.2f\n", shortestWaitTime);
}

int main(){
    int n;
    int maxTimeQuantum = 0;
    //float check = 1;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    struct Process processes[n];
    //輸入進程資訊
    for(int i = 0;i<n;i++){
        printf("Enter the arrival time and burst time of process %d: ", i+1);
        scanf("%d %d",&processes[i].arrival_time,&processes[i].burst_time);
        if(processes[i].burst_time > maxTimeQuantum){
            maxTimeQuantum = processes[i].burst_time;
        }
        processes[i].id = i+1;
    }
    // //計算最佳的time quantum
    // while(check/(n+1) < 0.8){
    //     check = 1;
    //     timeQuantum++;
    //     for(int i = 0;i<n;i++){
    //          if(timeQuantum >= processes[i].burst_time){
    //                 check++;
    //          }
    //     }
    // }
    // printf("timeQuantum: %d\n",timeQuantum);
    calculateAvgTimes(processes, n, maxTimeQuantum);

    return 0;
}