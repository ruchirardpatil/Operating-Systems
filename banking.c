/*
ASSIGNMENT-3 (CPU Scheduling)
Ruchira Patil
G2 Batch
PG 21
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIM 100					// lets assume 100 to be types of resources the os can dispose


// P-Process, R-Resource
int max[LIM][LIM];				// [i,j] then Pi can demand Rj max this much
int allocated[LIM][LIM];		// [i,j] then Pi currently has this many of Rj allocated
int need[LIM][LIM];				// [i,j] then Pi needs this much more of Rj
int available[LIM];				// [j] Rj max available resource of this type

int safe[LIM];					// stores safe sequence

int proc;						// no of processes in total
int res;						// no of resources in total

void init();					// this initiates a case of reource allocation
void display();					// this displays the current allocation in tabular format
bool checkSafety();				// safety algorithm for checking if system is safe
void rr_alloc();				// resource request allocation when additional resource is requested

int main()
{
	printf("\nThis program depicts various deadlock avoidance algorithms\n\n");
	printf("Enter no. of processes	");			// no of processes in OS
	scanf("%d", &proc);
	printf("Enter no. of resources	");			// no of resources, not its instances
	scanf("%d", &res);
	int choice = 10;
	bool isSafe;
	while(choice != 0)
	{
		printf("\nPlease enter your choice:\n1. Create base instance\n2. Request additional resources\n3. Display a Snapshot\n4. Check system safety\n5. Exit\n");
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:
				init();						// must use this to create basic forma
				break;
			case 2:
				rr_alloc();					// allocate additional resources all the while ensuring safety
				break;
			case 3: 
				display();					// display resource usage snap
				break;
			case 4:
				isSafe = checkSafety();		// if system is safe
				if(isSafe == false)
				{
					printf("%s\n", "System is not in safe mode.");
				} else {
					printf("%s\n", "System is in safe mode");
					printf("Safe Sequence : ");
					for(int i=0; i<proc; i++)
					{
						printf("P%d", safe[i]);
						if(i == proc-1) printf("\n");
						else printf("->");
					}
				}
				break;
			case 5:
				printf("\nEXITED!\n");
				choice = 0;
				break;
			default:
				printf("\nThats an incorrect choice, Please try again later.\n");
				break;
		}
	}
	return 0;
}

void init() {
	printf("\nEnter the total resource instances for each type of resource:\n");
	for (int i=0; i<res; i++) {
		printf("Instances of Resource #%d are:\t", i);
		scanf("%d", &available[i]);				// instances of each resource available
	}
	printf("\nNow enter the maximum resource each process can ask for:\n");
	for (int i=0; i<proc; i++) {
		printf("Process %d\n", i);
		for (int j=0; j<res; j++) {
			printf("No of instance of resource #%d it requires:\t", j);
			scanf("%d", &max[i][j]);			// max instance of each resource a process can ask for
		}
	}
	printf("\nNow enter the resources allocated to each process:\n");
	for (int i=0; i<proc; i++) {
		printf("Process %d\n", i);
		for (int j=0; j<res; j++) {
			printf("Instance of resource #%d allocated:\t", j);
			scanf("%d", &allocated[i][j]);		// currently allocated instances of resources	
			need[i][j] = max[i][j] - allocated[i][j];			// need is max - allocated, amount it can still ask
			available[j] = available[j] - allocated[i][j];		// available is total - allocated
		}
	}
}

void display() {
	printf("\nCurrent Snapshot is:\n");
	for(int i=0; i<proc; i++) {
		printf("\n\nProcess %d\n", i);
		printf("-------------\n");
		printf("Allocated\n");
		for(int j=0; j<res; j++) {
			printf("R%d - %d", j, allocated[i][j]);
			if (j != res-1) printf(", ");
		}
		printf("\nMaximum\n");
		for(int j=0; j<res; j++) {
			printf("R%d - %d", j, max[i][j]);
			if (j != res-1) printf(", ");
		}
		printf("\nNeed\n");
		for(int j=0; j<res; j++) {
			printf("R%d - %d", j, need[i][j]);
			if (j != res-1) printf(", ");
		}
	}
	printf("\n\nAVAILABLE:\n");
	for(int j=0; j<res; j++) {
		printf("R%d - %d", j, available[j]);
		if (j != res-1) printf(", ");
	}
	printf("\n");
}

void rr_alloc() {
	int request[100] = {0};				// we init this to 0 because we actively add them to pre-exiting values
	int pid;
	printf("\nPlease enter the Process ID ?\t");		// we only add resources for one
	scanf("%d", &pid);
	for(int i=0; i<res; i++) {
		printf("Additional resource #%d required ?\t", i);
		scanf("%d", &request[i]);						// ask resources for all available instances
		if(request[i] > need[pid][i]) {			// incase it goes beyond what it can ask for at max
			printf("\nThis process is requesting more resources than the max it can.\n");
			return;
		} else if (request[i] > available[i]) {		// incase it goes beyond what can be allocated
			printf("\nThis process is requesting resources not available. It enters WAIT state.\n");
			return;
		}
	}
	for(int i=0; i<res; i++) {
		available[i] = available[i] - request[i];			// assume resources are allocated
		allocated[pid][i] = allocated[pid][i] + request[i];
		need[pid][i] = need[pid][i] - request[i];
	}
	bool check = checkSafety();				// check if system is safe after allocation
	if(check == false) {
		for(int i=0; i<res; i++) {							// system is unsafe, restore original values
			available[i] = available[i] + request[i];
			allocated[pid][i] = allocated[pid][i] - request[i];
			need[pid][i] = need[pid][i] + request[i];
		}
		printf("\nSystem is not safe, request denied. Process enters WAIT. System restored.\n");
	} else {
		printf("\nRequest Granted.\n");
	}
}

bool checkSafety() {
	bool finish[LIM] = {false};			// initially we have visited none
	int work[LIM];
	for (int i=0; i<res; i++) {
		work[i] = available[i];			// a copy of available resources
	}
	int count = 0;
	while (count < proc) {				// while all the process are not finished i.e. not visited
		bool found = false;
		for (int p = 0; p < proc; p++) { 		// processes which are not finished and their needs need to satisfied with work resources
            if (finish[p] == false) { 			// only process if this is not finished else go to next
                int j; 
                for (j = 0; j < res; j++) {
                    if (need[p][j] > work[j]) break; 	// confirm that all needs of this process is less than available resources
                }
                if (j == res) { 					// if all resources are indeed less
                    for (int k = 0 ; k < res ; k++) {
                        work[k] += allocated[p][k]; 		// free up the resources so that next process may access them
                    }
                    safe[count++] = p; 	// add this process to safe list and increment count
                    finish[p] = true; 	// this process is in safe list
                    found = true; 		// push to safe list
                } 
            } 
        } 
        if (found == false) {		// if we could not find the next process in safe sequence
        	return false;
        }
	}
	return true;
}

/**	OUTPUT
This program depicts various deadlock avoidance algorithms

Enter no. of processes ?        5
Enter no. of resources ?        3

Please enter your choice:
1. Create base instance
2. Request additional resources
3. Display a Snapshot
4. Check system safety
5. Exit
1

Enter the total resource instances for each type of resource:
Instances of Resource #0 are:   10
Instances of Resource #1 are:   5
Instances of Resource #2 are:   7

Now enter the maximum resource each process can ask for:
Process 0
No of instance of resource #0 it requires:      7
No of instance of resource #1 it requires:      5
No of instance of resource #2 it requires:      3
Process 1
No of instance of resource #0 it requires:      3
No of instance of resource #1 it requires:      2
No of instance of resource #2 it requires:      2
Process 2
No of instance of resource #0 it requires:      9
No of instance of resource #1 it requires:      0
No of instance of resource #2 it requires:      2
Process 3
No of instance of resource #0 it requires:      2
No of instance of resource #1 it requires:      2
No of instance of resource #2 it requires:      2
Process 4
No of instance of resource #0 it requires:      4
No of instance of resource #1 it requires:      3
No of instance of resource #2 it requires:      3

Now enter the resources allocated to each process:
Process 0
Instance of resource #0 allocated:      0
Instance of resource #1 allocated:      1
Instance of resource #2 allocated:      0
Process 1
Instance of resource #0 allocated:      2
Instance of resource #1 allocated:      0
Instance of resource #2 allocated:      0
Process 2
Instance of resource #0 allocated:      3
Instance of resource #1 allocated:      0
Instance of resource #2 allocated:      2
Process 3
Instance of resource #0 allocated:      2
Instance of resource #1 allocated:      1
Instance of resource #2 allocated:      1
Process 4
Instance of resource #0 allocated:      0
Instance of resource #1 allocated:      0
Instance of resource #2 allocated:      2

Please enter your choice:
1. Create base instance
2. Request additional resources
3. Display a Snapshot
4. Check system safety
5. Exit
3

Current Snapshot is:


Process 0
-------------
Allocated
R0 - 0, R1 - 1, R2 - 0
Maximum
R0 - 7, R1 - 5, R2 - 3
Need
R0 - 7, R1 - 4, R2 - 3

Process 1
-------------
Allocated
R0 - 2, R1 - 0, R2 - 0
Maximum
R0 - 3, R1 - 2, R2 - 2
Need
R0 - 1, R1 - 2, R2 - 2

Process 2
-------------
Allocated
R0 - 3, R1 - 0, R2 - 2
Maximum
R0 - 9, R1 - 0, R2 - 2
Need
R0 - 6, R1 - 0, R2 - 0

Process 3
-------------
Allocated
R0 - 2, R1 - 1, R2 - 1
Maximum
R0 - 2, R1 - 2, R2 - 2
Need
R0 - 0, R1 - 1, R2 - 1

Process 4
-------------
Allocated
R0 - 0, R1 - 0, R2 - 2
Maximum
R0 - 4, R1 - 3, R2 - 3
Need
R0 - 4, R1 - 3, R2 - 1

AVAILABLE:
R0 - 3, R1 - 3, R2 - 2

Please enter your choice:
1. Create base instance
2. Request additional resources
3. Display a Snapshot
4. Check system safety
5. Exit
2

Please enter the Process ID ?   0
Additional resource #0 required ?       1
Additional resource #1 required ?       0
Additional resource #2 required ?       2

System is not safe, request denied. Process enters WAIT. System restored.

Please enter your choice:
1. Create base instance
2. Request additional resources
3. Display a Snapshot
4. Check system safety
5. Exit
2

Please enter the Process ID ?   1
Additional resource #0 required ?       1
Additional resource #1 required ?       0
Additional resource #2 required ?       2

Request Granted.

Please enter your choice:
1. Create base instance
2. Request additional resources
3. Display a Snapshot
4. Check system safety
5. Exit
3

Current Snapshot is:


Process 0
-------------
Allocated
R0 - 0, R1 - 1, R2 - 0
Maximum
R0 - 7, R1 - 5, R2 - 3
Need
R0 - 7, R1 - 4, R2 - 3

Process 1
-------------
Allocated
R0 - 3, R1 - 0, R2 - 2
Maximum
R0 - 3, R1 - 2, R2 - 2
Need
R0 - 0, R1 - 2, R2 - 0

Process 2
-------------
Allocated
R0 - 3, R1 - 0, R2 - 2
Maximum
R0 - 9, R1 - 0, R2 - 2
Need
R0 - 6, R1 - 0, R2 - 0

Process 3
-------------
Allocated
R0 - 2, R1 - 1, R2 - 1
Maximum
R0 - 2, R1 - 2, R2 - 2
Need
R0 - 0, R1 - 1, R2 - 1

Process 4
-------------
Allocated
R0 - 0, R1 - 0, R2 - 2
Maximum
R0 - 4, R1 - 3, R2 - 3
Need
R0 - 4, R1 - 3, R2 - 1

AVAILABLE:
R0 - 2, R1 - 3, R2 - 0

Please enter your choice:
1. Create base instance
2. Request additional resources
3. Display a Snapshot
4. Check system safety
5. Exit
5

GOODBYE !!!

*/
