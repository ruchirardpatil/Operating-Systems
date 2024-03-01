/*
ASSIGNMENT-2 (Process Control)
Ruchira Patil
G2 Batch
PG 21
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void input(int*, int);      
void display(int*, int);    // display contetns of array
void sort(int*, int, int);  // sort either ascending or descending

int main()
{
    printf("\nThis program attempts to create a child process using fork.\n\n");
    int n;
    printf("Enter the number of elements\n");
    scanf("%d", &n);
    int a[n];
    input(a, n);
    printf("\nThe array to be sorted is : \n");
    display(a, n);
    printf("\n");
    int id = fork();           						// creates a child process executing from below
    if (id == 0)
	{
        printf("\nChild Process\n");
        sort(a, n, 0);          					// ascending is 1
        printf("The sorted array is:\n");
        display(a, n);
        exit(0);           							// with status 0 (success)
    } else
	{
        int status;
        wait(&status);      						// wait for child process status
        if ( WIFEXITED(status) )
		{ 											// if exited
            int exit_status = WEXITSTATUS(status);  // print status
            printf("\n\nParent Process\n");
            printf("Exit status of the child was %d\n", exit_status);
        }
        sort(a, n, 1);  							// descending
        printf("The sorted array is:\n");
        display(a, n);
    }
    printf("\n");
    return 0;
}

void input(int *a, int n)
{
    for(int i=0; i<n; i++)
	{
        printf("Enter value of element #%d\t", i+1);
        scanf("%d", &a[i]);
    }
}

void display(int *a, int n)
{
    for(int i=0; i<n; i++)
	{
        printf("%d ", a[i]);
    }
}

void sort(int *a, int n, int c)
{ 														// c 1 is ascend 0 is descend
    for(int i=0; i<n; i++)
	{
		for(int j=0; j<n-1; j++)
		{
			if( a[j] > a[j+1] && c == 1)
			{
				int temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			} else if ( a[j] < a[j+1] && c == 0)
			{
                int temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
            }
		}
	}
}

/* OUTPUT:-

This program attempts to create a child process using fork.

Enter the number of elements
5
Enter value of element #1       15
Enter value of element #2       45
Enter value of element #3       09
Enter value of element #4       34
Enter value of element #5       27

The array to be sorted is :
15 45 09 34 27

Child Process
The sorted array is:
45 34 27 15 09

Parent Process
Exit status of the child was 0
The sorted array is:
09 15 27 34 45

*/
