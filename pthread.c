/*
ASSIGNMENT-5 (Implementing Multithreading using POSIX standard)
Ruchira Patil
G2 Batch
PG 21
*/

#include<stdio.h>
#include<pthread.h>

#define LIM 50

pthread_t thread1,thread2;	// bi threaded program

struct arg_struct
{	// we cannot pass multiple arguments to pthread, so we use struct
    int arg1[LIM];
    int arg2;
    int id;
};

void add(void*);
void sub(void*);
void mul(void*);
void divi(void*);

int main()
{
    int n;
    printf("\nThis program utilizes two parallel threads to perform operation on n numbers.\n");
    printf("How many numbers do you want to operate on ?\t");
    scanf("%d", &n);
    if(n%2 != 0)
	{
        printf("Only even nos, hence considering %d\n", n-1);
        n -= 1;
    }
    int act = n/2;				// mid point ... half will go to either thread
    struct arg_struct t1, t2;
    t1.arg2 = act;				// this will store the no of times to iterate while adding
    t1.id = 1;
    t2.arg2 = act;
    t2.id = 2;
    for(int i=0; i<n; i++)
	{
        printf("Enter No.%d :\t", i+1);
        if(i<act)
            scanf("%d", &t1.arg1[i]);		// first half of args
        else
            scanf("%d", &t2.arg1[i-act]);	// second half of args
    }

    int choice = 10;
    while(choice != 0)
	{
        printf("\nPlease enter the task to perform:\n1. Addition\n2. Substraction\n3. Multiplication\n4. Division\n5. Exit\n");
        scanf("%d", &choice);
        switch(choice)
		{
        	case 1:
        		pthread_create(&thread1, NULL,(void*)add, (void*)&t1);		// create first thread
    			pthread_create(&thread2, NULL,(void*)add, (void*)&t2);		// create second thread
    			pthread_join(thread1, NULL);								// suspend till end of t1
    			pthread_join(thread2, NULL);								// suspend till end of t2
    			printf("\nThe Final Sum = %d\n", t1.arg1[act+1] + t2.arg1[act+1]);
    			break;
    		case 2:
        		pthread_create(&thread1, NULL,(void*)sub, (void*)&t1);		// create first thread
    			pthread_create(&thread2, NULL,(void*)sub, (void*)&t2);		// create second thread
    			pthread_join(thread1, NULL);								// suspend till end of t1
    			pthread_join(thread2, NULL);								// suspend till end of t2
    			printf("\nThe Final Substraction = %d\n", t1.arg1[act+1] - t2.arg1[act+1]);
    			break;
    		case 3:
        		pthread_create(&thread1, NULL,(void*)mul, (void*)&t1);		// create first thread
    			pthread_create(&thread2, NULL,(void*)mul, (void*)&t2);		// create second thread
    			pthread_join(thread1, NULL);								// suspend till end of t1
    			pthread_join(thread2, NULL);								// suspend till end of t2
    			printf("\nThe Final Multiplication = %d\n", t1.arg1[act+1] * t2.arg1[act+1]);
    			break;
    		case 4:
        		pthread_create(&thread1, NULL,(void*)divi, (void*)&t1);		// create first thread
    			pthread_create(&thread2, NULL,(void*)divi, (void*)&t2);		// create second thread
    			pthread_join(thread1, NULL);								// suspend till end of t1
    			pthread_join(thread2, NULL);								// suspend till end of t2
    			printf("\nThe Final Division = %d\n", t1.arg1[act+1] / t2.arg1[act+1]);
    			break;
    		case 5:
    			printf("\nExited!!!\n");
    			choice = 0;
    			break;
    		default:
    			printf("\nYou have entered an incorrect choice, try again.\n");
    			break;
        }
    }
    return 0;
}

void add(void *args){										// addition
    struct arg_struct *temp = (struct arg_struct*)args;		// convert from vois to struct arg then assign to pointer
    temp->arg1[temp->arg2+1] = temp->arg1[0];
    for(int i=1; i<temp->arg2; i++)
	{
        temp->arg1[temp->arg2+1] += temp->arg1[i];			// add on next value
    }
    printf("\nAddition result from Thread #%d is %d\n", temp->id, temp->arg1[temp->arg2+1]);
}

void sub(void *args)
{	// substraction, it cant be implemented properly so instead, it subs all from first
    struct arg_struct *temp = (struct arg_struct*)args;
    temp->arg1[temp->arg2+1] = temp->arg1[0];
    for(int i=1; i<temp->arg2; i++)
	{
        temp->arg1[temp->arg2+1] -= temp->arg1[i];
    }
    printf("\nSubstraction result from Thread #%d is %d\n", temp->id, temp->arg1[temp->arg2+1]);
}

void mul(void *args)
{	// multiplication, again with first
    struct arg_struct *temp = (struct arg_struct*)args;
    temp->arg1[temp->arg2+1] = temp->arg1[0];
    for(int i=1; i<temp->arg2; i++)
	{
        temp->arg1[temp->arg2+1] *= temp->arg1[i];
    }
    printf("\nMultiplication result from Thread #%d is %d\n", temp->id, temp->arg1[temp->arg2+1]);
}

void divi(void *args)
{	// division again with first
    struct arg_struct *temp = (struct arg_struct*)args;
    temp->arg1[temp->arg2+1] = temp->arg1[0];
    for(int i=1; i<temp->arg2; i++)
	{
        temp->arg1[temp->arg2+1] /= temp->arg1[i];
    }
    printf("\nDivision result from Thread #%d is %d\n", temp->id, temp->arg1[temp->arg2+1]);
}

/* OUTPUT :-

This program utilizes two parallel threads to perform operation on n numbers.
How many numbers do you want to operate on ?   4
Enter No.1 :    23
Enter No.2 :    10
Enter No.3 :    19
Enter No.4 :    8

Please enter the task to perform:
1. Addition
2. Substraction
3. Multiplication
4. Division
5. Exit
1

Addition result from Thread #1 is 33

Addition result from Thread #2 is 27

The Final Sum = 60

Please enter the task to perform:
1. Addition
2. Substraction
3. Multiplication
4. Division
5. Exit
2

Substraction result from Thread #1 is 13

Substraction result from Thread #2 is 11

The Final Substraction = 2

Please enter the task to perform:
1. Addition
2. Substraction
3. Multiplication
4. Division
5. Exit
3

Multiplication result from Thread #1 is 230

Multiplication result from Thread #2 is 152

The Final Multiplication = 34960

Please enter the task to perform:
1. Addition
2. Substraction
3. Multiplication
4. Division
5. Exit
4

Division result from Thread #1 is 2

Division result from Thread #2 is 2

The Final Division = 1

Please enter the task to perform:
1. Addition
2. Substraction
3. Multiplication
4. Division
5. Exit
5

Exited!!!

*/
