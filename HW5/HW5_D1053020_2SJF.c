#include <stdio.h>
#include <stdlib.h>

// Add priority to Process
typedef struct
{
    int process_id;
    int burst_time;
    int priority;
} Process;

void sortProcesses(Process processes[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].priority > processes[j + 1].priority)
            {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
            else if (processes[j].priority == processes[j + 1].priority)
            {
                if (processes[j].burst_time > processes[j + 1].burst_time)
                {
                    Process temp = processes[j];
                    processes[j] = processes[j + 1];
                    processes[j + 1] = temp;
                }
            }
        }
    }
}

void calculateTurnaroundTime(Process processes[], int n, int turnaround_time[])
{
    int waiting_time[n];
    waiting_time[0] = 0;
    int total_waiting_time = 0;

    for (int i = 1; i < n; i++)
    {
        waiting_time[i] = waiting_time[i - 1] + processes[i - 1].burst_time;
    }

    for (int i = 0; i < n; i++)
    {
        total_waiting_time += waiting_time[i];
    }

    printf("SJF Average Waiting Time = %.2f\n", (float)total_waiting_time / n);

    for (int i = 0; i < n; i++)
    {
        turnaround_time[i] = waiting_time[i] + processes[i].burst_time;
    }
}

void calculateAverageTime(Process processes[], int n)
{
    int turnaround_time[n];
    calculateTurnaroundTime(processes, n, turnaround_time);

    float total_turnaround_time = 0;

    for (int i = 0; i < n; i++)
    {
        total_turnaround_time += turnaround_time[i];
    }

    float avg_turnaround_time = total_turnaround_time / n;
    //printf("Average Turnaround Time = %.2f\n", avg_turnaround_time);
}

int main()
{
    int n;
    printf("Enter the number of processes(max 5): ");
    scanf("%d", &n);
    if (n < 1 || n > 5)
    {
        printf("Invalid number of processes\n");
        return 1;
    }

    Process processes[n];

    for (int i = 0; i < n; i++)
    {
        processes[i].process_id = i + 1;
        printf("Enter the burst time and priority for process %d: ", i + 1);
        scanf("%d %d", &processes[i].burst_time, &processes[i].priority);
    }

    printf("Before sort: Process ID\tBurst Time\tPriority\n");
    for (int i = 0; i < n; i++)
    {
        printf("\t\t%d\t\t%d\t\t%d\n", processes[i].process_id, processes[i].burst_time, processes[i].priority);
    }

    sortProcesses(processes, n);
    printf("After sort: Process ID\tBurst Time\tPriority\n");

    for (int i = 0; i < n; i++)
    {
        printf("\t\t%d\t\t%d\t\t%d\n", processes[i].process_id, processes[i].burst_time, processes[i].priority);
    }

    //顯示結果
    for(int i = 0; i < 50; i++){
        printf("-");
    }
    printf("\n");
    calculateAverageTime(processes, n);

    return 0;
}