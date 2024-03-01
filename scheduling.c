/*
ASSIGNMENT-3 (CPU Scheduling)
Ruchira Patil
G2 Batch
PG 21
*/

#include <stdio.h>
#include <stdlib.h>

struct process
{							// contents of a processor, equivalent to classes
	int p_no;				// process id
	int arrival_time; 	    // time of process enqueue
	int burst_time;			// execution time of process
	int turnaround_time;    // time from start to end of process
	int wait_time;			// time spent in ready state
};

int input(); 							// takes in no. of process to consider
void processInfo(struct process*, int); // takes in process information
void FCFS(struct process*, int); 		// first come first server
void SJF(struct process*, int);			// short job first (pre-emptive)

int main()
{
	printf("\nThis programs emulates CPU Scheduling via two methods : SJF and FCFS\n\n");
	int no = input();
	struct process p[no];
	int choice = 10;
	while (choice != 0)
	{
		printf("\nPlease enter your choice:\n1. First Come First Serve Algorithm\n2. Shortest Job First (pre-emptive)\n");
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:
				processInfo(p, no);
				FCFS(p, no);
				break;
			case 2: 
				processInfo(p, no);
				SJF(p, no);
				break;
			default: 
				printf("%s\n", "Invalid choice");
				break;
		}
	}
	return 0;
}

int input()
{
	int a;
	printf("\nPlease enter the number of process\t");		// no of process to consider
	scanf("%d", &a);
	return a;
}

void processInfo(struct process *p, int lim)
{
	for (int i=0; i<lim; i++)
	{														// take input for each process
		printf("\nProcess %d\n", i+1);
		printf("Enter arrival time\t");
		scanf("%d", &p[i].arrival_time);
		printf("Enter burst time\t");
		scanf("%d", &p[i].burst_time);
		p[i].p_no = i+1;
	}
	struct process temp;
	for(int i=0; i<lim-1; i++)
	{									// sort the processes via arrival time
		for(int j=0; j<lim-1-i; j++)
		{								// bubble sort for simplicity
			if(p[j].arrival_time > p[j+1].arrival_time)
			{
				temp = p[j];
				p[j] = p[j+1];
				p[j+1] = temp;			// simple swap
			}
		}
	}
}

void FCFS(struct process *p, int n)
{
	int total_wt = 0;					  // total wait
	int total_tat = 0;					  // total turnaround

	int current_time = p[0].arrival_time; //initialize current time to the arrival time of first process

	printf("\nGantt Chart:\n");
	printf("%d", current_time);
	//for each process calculate the turnaround time and waiting time and display it in the form of gantt chart
	for(int i=0; i<n; i++)
	{	
		current_time += p[i].burst_time;
		printf(" <---P%d---> %d", p[i].p_no, current_time);
		p[i].turnaround_time = current_time - p[i].arrival_time;
		p[i].wait_time = p[i].turnaround_time - p[i].burst_time;
		total_tat += p[i].turnaround_time;
		total_wt += p[i].wait_time;
		if(i < n-1 && current_time < p[i+1].arrival_time)
		{	//checks if CPU is idle
			current_time = p[i+1].arrival_time;
			printf(" <---IDLE---> %d", current_time);
		}
	}

	//prints waiting time and turnaround time for all processes in tabular format
	printf("\n\nProcess\tArrival\tBurst\tTurnaround\tWait\n");
	printf("\tTime\tTime\tTime\t\tTime\n");
	for(int i=0; i<n; i++)
	{
		printf("P%d\t%d\t%d\t%d\t\t%d\n", p[i].p_no, p[i].arrival_time, p[i].burst_time, p[i].turnaround_time, p[i].wait_time);
	}
	float avg_tat=(float)total_tat/n;
	float avg_wt=(float)total_wt/n;
	printf("\nTotal Turn around time = %d\n",total_tat);
	printf("\nAverage Turn around time = %.2f\n",avg_tat);
	printf("\nTotal Waiting time = %d\n",total_wt);
	printf("\nAverage Waiting time = %.2f\n",avg_wt);
}

void SJF(struct process *p, int n)
{
	int remaining_time[n];			      // burst time backup
	int total_wt = 0;			       	  // total wait
	int total_tat = 0;					  // total turnaround

	int current_time = p[0].arrival_time; //initialize current time to the arrival time of first process
	int cnt = 0;						  // processes parsed
	int least_rt = 9999; 				  // least remaining time
	int k;				 				  // least index

	int i=0;							  // counter
	//storing the burst time of each process in a separate array of remaining time
	for(int j=0; j<n; j++)
	{
		remaining_time[j] = p[j].burst_time;
	}
	printf("\nGantt Chart:\n");
	//for each process calculate the turnaround time and waiting time and display it in the form of gantt chart
	while(i != n)
	{//while all the processes are not selected by the processor

		for(int j=0; j<n; j++)
		{
			if(p[j].arrival_time <= current_time && remaining_time[j] >= 0)
			{
				cnt++;
				if(remaining_time[j] < least_rt && remaining_time[j]>0)
				{	//select the process having least remaining time
					k = j;
					least_rt = remaining_time[j];	
				}
			}
		}

		if(cnt == n)
		{		//if all processes arrived behave as non-preemptive SJF
				printf("%d <---P%d---> ", current_time, p[k].p_no);
				current_time = current_time + remaining_time[k];
				remaining_time[k] = 0;
				p[k].turnaround_time = current_time - p[k].arrival_time;
				p[k].wait_time = p[k].turnaround_time - p[k].burst_time;
				//calculate waiting time and turnaround time for each process as they get terminated
				total_tat += p[k].turnaround_time;
				total_wt += p[k].wait_time;
				i++;
		} else
		{ 									//if all processes have not yet arrived
			printf("%d <---P%d---> ", current_time, p[k].p_no);
			current_time += 1;				//increase current time by 1 and check whether the newly arrived process has shorter burst 
			remaining_time[k] -= 1;			//time than the remaining time of the executing process
			if(remaining_time[k] == 0)
			{
				p[k].turnaround_time = current_time - p[k].arrival_time;
				p[k].wait_time = p[k].turnaround_time - p[k].burst_time;
				total_tat += p[k].turnaround_time;
				total_wt += p[k].wait_time;
				i++;
			}
		}

		cnt = 0;
		least_rt = 9999;
	}

	printf("%d",current_time);
	//prints waiting time and turnaround time for all processes in tabular format
	printf("\n\nProcess\tArrival\tBurst\tTurnaround\tWait\n");
	printf("\tTime\tTime\tTime\t\tTime\n");
	for(int i=0; i<n; i++)
	{
		printf("P%d\t%d\t%d\t%d\t\t%d\n", p[i].p_no, p[i].arrival_time, p[i].burst_time, p[i].turnaround_time, p[i].wait_time);
	}
	float avg_tat=(float)total_tat/n;
	float avg_wt=(float)total_wt/n;
	printf("\nTotal Turn around time = %d\n",total_tat);
	printf("\nAverage Turn around time = %.2f\n",avg_tat);
	printf("\nTotal Waiting time = %d\n",total_wt);
	printf("\nAverage Waiting time = %.2f\n",avg_wt);
}

/*OUTPUT :-

This programs emulates CPU Scheduling via two methods : SJF and FCFS


Please enter the number of process    5

Please enter your choice:
1. First Come First Serve Algorithm
2. Shortest Job First (pre-emptive)
1

Process 1
Enter arrival time    2
Enter burst time      2

Process 2
Enter arrival time    0
Enter burst time      1

Process 3
Enter arrival time    2
Enter burst time      3

Process 4
Enter arrival time    3
Enter burst time      5

Process 5
Enter arrival time    4
Enter burst time      4

Gantt Chart:
0 <---P2---> 1 <---IDLE---> 2 <---P1---> 4 <---P3---> 7 <---P4---> 12 <---P5---> 16

Process Arrival Burst   Turnaround      Wait
        Time    Time    Time    		Time
P2      0       1       1               0
P1      2       2       2               0
P3      2       3       5               2
P4      3       5       9               4
P5      4       4       12              8

Total Turn around time = 29

Average Turn around time = 5.80

Total Waiting time = 14

Average Waiting time = 2.80

Please enter your choice:
1. First Come First Serve Algorithm
2. Shortest Job First (pre-emptive)
2

Process 1
Enter arrival time    2
Enter burst time      1

Process 2
Enter arrival time    1
Enter burst time      5

Process 3
Enter arrival time    4
Enter burst time      1

Process 4
Enter arrival time    0
Enter burst time      6

Process 5
Enter arrival time    2
Enter burst time      3

Gantt Chart:
0 <---P4---> 1 <---P4---> 2 <---P1---> 3 <---P5---> 4 <---P3---> 5 <---P5---> 7 <---P4---> 11 <---P2---> 16

Process Arrival Burst   Turnaround      Wait
        Time    Time    Time    		Time
P4      0       6       11              5
P2      1       5       15              10
P1      2       1       1               0
P5      2       3       5               2
P3      4       1       1               0

Total Turn around time = 33

Average Turn around time = 6.60

Total Waiting time = 17

Average Waiting time = 3.40

Please enter your choice:
1. First Come First Serve Algorithm
2. Shortest Job First (pre-emptive)
3

*/
