/*
ASSIGNMENT-6 (Demand Paging - Page Replacement Algorithms)
Ruchira Patil
G2 Batch
PG 21
*/

#include <iostream>
using namespace std;

#define MAX 50						// lets assume max length of ref string is 50

void FIFO(int, int*, int);
void LRU(int, int*, int);
void checkBelady(int, int*, int);

class queue
{
	int lim;						// frame
	int tot;						// total reference string without repeat 
	int data[MAX];					// queue storage
	int frequency[MAX][MAX];		// recent call of each reference string
	int front;						// not circular since we will refer deleted elements
	int rear;
	public:
		queue(int, int*, int);
		bool isFull();
		bool isEmp();
		bool isPresent(int, int);
		void enqueue(int);
		void dequeue();
		int findPos(int);
		void sortByDemand();
		void print();
		void frequencyCount();
};

queue :: queue(int frame, int *det, int total)
{	// will populate refernce string frequency
	lim = frame;
	int count = 0;
	for (int i=0; i<total; i++)
	{
		int flag = 0;
		for (int j=0; j<i; j++)
		{
			if (frequency[j][0] == det[i]) flag = 1;		// so that we dont repeat same characters
		}
		if (flag == 0)
		{
			frequency[count][0] = det[i];
			frequency[count][1] = 0;
			count++;
		}
	}
	tot = count;
	front = -1;												// basic queue init
	rear = -1;
}

bool queue :: isFull()
{	// true if queue is full, ie if no of active elements = frame size
	if (rear-front == lim) return true;
	return false;
}

bool queue :: isEmp()
{	// true if queue is empty
	if (front == rear) return true;
	else return false;
}

bool queue :: isPresent(int temp, int ind)
{	// will check if an element is present, if requested will increment it's frequency too
	int pos = findPos(temp);
	if (pos != -1) frequency[pos][1] = ind;
	for (int i=front+1; i<=rear; i++)
	{
		if (data[i] == temp) return true;
	}
	return false;				// if not found, [will never happen]
}

void queue :: enqueue(int temp)
{	// add element to queue
	rear++;
	data[rear] = temp;
}

void queue :: dequeue()
{	// remove from consideration, not delete
	front++;
}

int queue :: findPos(int a)
{	// finds out position of element in frequency table
	for (int i=0; i<tot; i++)
	{
		if (frequency[i][0] == a) return i;
	}
	return -1;
}

void queue :: sortByDemand()
{	// sorts based on recent call, the least frequently called will be in front
	int count = 0;					// since we require no of iterations and front won't always start from 0
	for (int i=front+1; i<=rear-1; i++)
	{
		for (int j=front+1; j<=rear-count-1; j++)
		{	// basic bubble sort
			int ind1 = findPos(data[j]);
			int ind2 = findPos(data[j+1]);
			if (frequency[ind1][1] > frequency[ind2][1])
			{	// swap
				int temp = data[j];
				data[j] = data[j+1];
				data[j+1] = temp;
			}
		}
		count++;
	}
}

void queue :: frequencyCount()
{	// prints frequency of active elements in queue
	for (int i=front+1; i<=rear; i++)
	{
		int ind = findPos(data[i]);
		cout<<frequency[ind][0]<<" (Called - "<<frequency[ind][1]<<") ";
	}
	cout<<endl;
}

void queue :: print()
{				// prints active elements in queue
	for (int i=front+1; i<=rear; i++)
	{
		cout<<data[i]<<" ";
	}
	cout<<endl;
}

int main()
{
	cout<<"\nThis program implements page replacement algorithm for First In First Out and Least Recently Used\n"<<endl;
	int n;
	cout<<"Enter the number of reference strings ?\t";
	cin>>n;
	int arr[n];
	for (int i=0; i<n; i++)
	{
		cout<<"Enter Ref String #"<<i+1<<"\t";
		cin>>arr[i];
	}
	int frame;
	cout<<"Enter frame size ?\t";
	cin>>frame;
	cout<<endl;
	cout<<"Reference String is : ";
	for (int i=0; i<n; i++)
	{
		cout<<arr[i]<<" ";
	}
	cout<<endl<<"Frame size is : "<<frame<<endl;
	cout<<"\nFirst In First Out\n"<<endl;
	FIFO(n, arr, frame);
	checkBelady(n, arr, frame);
	cout<<"\n\nLeast Recently Used\n"<<endl;
	LRU(n, arr, frame);

	cout<<"\n\nGoodbye !!!"<<endl;
	return 0;
}

void FIFO(int n, int *arr, int frame)
{
	queue *q1 = new queue(frame, arr, n);
	int fault = 0;
	for (int i=0; i<n; i++)
	{
		cout<<"Reference : "<<arr[i]<<endl;
		if (q1->isPresent(arr[i], i) == false)
		{
			cout<<"> Fault Occured"<<endl;					// if not found in current page
			fault += 1;
			if (q1->isFull() == true) q1->dequeue();		// remove topmost element
			q1->enqueue(arr[i]);			// add new element
		} else
		{
			cout<<"> Already Present"<<endl;
		}
		cout<<"> Queue : ";
		q1->print();
		cout<<endl;
	}
	cout<<"\nTotal Faults: "<<fault<<endl;
}

void LRU(int n, int *arr, int frame)
{
	queue *q2 = new queue(frame, arr, n);
	int fault = 0;
	for (int i=0; i<n; i++)
	{
		cout<<"Reference : "<<arr[i]<<endl;
		if (q2->isPresent(arr[i], i) == false)
		{
			cout<<"> Fault Occured"<<endl;		// if not found in current page
			fault += 1;
			if (q2->isFull() == true)
			{							// if full, first sort so that deque will remove least requested element
				q2->sortByDemand();		// sort by request
				q2->dequeue();			// remove topmost element
			}
			q2->enqueue(arr[i]);		// add new element
		} else
		{
			cout<<"> Already Present"<<endl;
		}
		cout<<"> Frequency : ";
		q2->frequencyCount();		// print frequency of present elements too
		cout<<"> Queue : ";
		q2->print();
		cout<<endl;
	}
	cout<<"\nTotal Faults: "<<fault<<endl;
}

void checkBelady(int n, int *arr, int frame)
{
	queue *q3 = new queue(frame, arr, n);
	queue *q4 = new queue(frame+1, arr, n);
	int f1 = 0, f2 = 0;
	for(int i=0; i<n; i++)
	{	// this anomaly occurs in case of fifo, increasing frames increases page faults too
		if (q3->isPresent(arr[i], i) == false)
		{
			f1 += 1;
			if (q3->isFull() == true) q3->dequeue();
			q3->enqueue(arr[i]);
		}
		if (q4->isPresent(arr[i], i) == false)
		{
			f2 += 1;
			if (q4->isFull() == true) q4->dequeue();
			q4->enqueue(arr[i]);
		}
	}
	if (f2 > f1)
	{
		cout<<"\nBélády’s anomaly is occuring. For "<<frame<<" frames "<<f1<<" faults occured. For "<<frame+1<<" frames "<<f2<<" faults occured"<<endl;
	} else
	{
		cout<<"\nBélády’s anomaly is not occuring."<<endl;
	}
}


/* OUTPUT :-

This program implements page replacement algorithm for First In First Out and Least Recently Used

Enter the number of reference strings ? 12
Enter Ref String #1     2
Enter Ref String #2     3
Enter Ref String #3     2
Enter Ref String #4     1
Enter Ref String #5     5
Enter Ref String #6     2
Enter Ref String #7     4
Enter Ref String #8     5
Enter Ref String #9     3
Enter Ref String #10    2
Enter Ref String #11    5
Enter Ref String #12    2
Enter frame size ?      4

Reference String is : 2 3 2 1 5 2 4 5 3 2 5 2
Frame size is : 4

First In First Out

Reference : 2
> Fault Occured
> Queue : 2

Reference : 3
> Fault Occured
> Queue : 2 3

Reference : 2
> Already Present
> Queue : 2 3

Reference : 1
> Fault Occured
> Queue : 2 3 1

Reference : 5
> Fault Occured
> Queue : 2 3 1 5

Reference : 2
> Already Present
> Queue : 2 3 1 5

Reference : 4
> Fault Occured
> Queue : 3 1 5 4

Reference : 5
> Already Present
> Queue : 3 1 5 4

Reference : 3
> Already Present
> Queue : 3 1 5 4

Reference : 2
> Fault Occured
> Queue : 1 5 4 2

Reference : 5
> Already Present
> Queue : 1 5 4 2

Reference : 2
> Already Present
> Queue : 1 5 4 2


Total Faults: 6

Bélády’s anomaly is not occuring.


Least Recently Used

Reference : 2
> Fault Occured
> Frequency : 2 (Called - 0)
> Queue : 2

Reference : 3
> Fault Occured
> Frequency : 2 (Called - 0) 3 (Called - 1)
> Queue : 2 3

Reference : 2
> Already Present
> Frequency : 2 (Called - 2) 3 (Called - 1)
> Queue : 2 3

Reference : 1
> Fault Occured
> Frequency : 2 (Called - 2) 3 (Called - 1) 1 (Called - 3)
> Queue : 2 3 1

Reference : 5
> Fault Occured
> Frequency : 2 (Called - 2) 3 (Called - 1) 1 (Called - 3) 5 (Called - 4)
> Queue : 2 3 1 5

Reference : 2
> Already Present
> Frequency : 2 (Called - 5) 3 (Called - 1) 1 (Called - 3) 5 (Called - 4)
> Queue : 2 3 1 5

Reference : 4
> Fault Occured
> Frequency : 1 (Called - 3) 5 (Called - 4) 2 (Called - 5) 4 (Called - 6)
> Queue : 1 5 2 4

Reference : 5
> Already Present
> Frequency : 1 (Called - 3) 5 (Called - 7) 2 (Called - 5) 4 (Called - 6)
> Queue : 1 5 2 4

Reference : 3
> Fault Occured
> Frequency : 2 (Called - 5) 4 (Called - 6) 5 (Called - 7) 3 (Called - 8)
> Queue : 2 4 5 3

Reference : 2
> Already Present
> Frequency : 2 (Called - 9) 4 (Called - 6) 5 (Called - 7) 3 (Called - 8)
> Queue : 2 4 5 3

Reference : 5
> Already Present
> Frequency : 2 (Called - 9) 4 (Called - 6) 5 (Called - 10) 3 (Called - 8)
> Queue : 2 4 5 3

Reference : 2
> Already Present
> Frequency : 2 (Called - 11) 4 (Called - 6) 5 (Called - 10) 3 (Called - 8)
> Queue : 2 4 5 3


Total Faults: 6


Goodbye !!!

---------------------------------

This program implements page replacement algorithm for First In First Out and Least Recently Used

Enter the number of reference strings ? 12
Enter Ref String #1     1
Enter Ref String #2     2
Enter Ref String #3     3
Enter Ref String #4     4
Enter Ref String #5     1
Enter Ref String #6     2
Enter Ref String #7     5
Enter Ref String #8     1
Enter Ref String #9     2
Enter Ref String #10    3
Enter Ref String #11    4
Enter Ref String #12    5
Enter frame size ?      3

Reference String is : 1 2 3 4 1 2 5 1 2 3 4 5
Frame size is : 3

First In First Out

Reference : 1
> Fault Occured
> Queue : 1

Reference : 2
> Fault Occured
> Queue : 1 2

Reference : 3
> Fault Occured
> Queue : 1 2 3

Reference : 4
> Fault Occured
> Queue : 2 3 4

Reference : 1
> Fault Occured
> Queue : 3 4 1

Reference : 2
> Fault Occured
> Queue : 4 1 2

Reference : 5
> Fault Occured
> Queue : 1 2 5

Reference : 1
> Already Present
> Queue : 1 2 5

Reference : 2
> Already Present
> Queue : 1 2 5

Reference : 3
> Fault Occured
> Queue : 2 5 3

Reference : 4
> Fault Occured
> Queue : 5 3 4

Reference : 5
> Already Present
> Queue : 5 3 4


Total Faults: 9

Bélády’s anomaly is occuring. For 3 frames 9 faults occured. For 4 frames 10 faults occured


Least Recently Used

Reference : 1
> Fault Occured
> Frequency : 1 (Called - 0)
> Queue : 1

Reference : 2
> Fault Occured
> Frequency : 1 (Called - 0) 2 (Called - 1)
> Queue : 1 2

Reference : 3
> Fault Occured
> Frequency : 1 (Called - 0) 2 (Called - 1) 3 (Called - 2)
> Queue : 1 2 3

Reference : 4
> Fault Occured
> Frequency : 2 (Called - 1) 3 (Called - 2) 4 (Called - 3)
> Queue : 2 3 4

Reference : 1
> Fault Occured
> Frequency : 3 (Called - 2) 4 (Called - 3) 1 (Called - 4)
> Queue : 3 4 1

Reference : 2
> Fault Occured
> Frequency : 4 (Called - 3) 1 (Called - 4) 2 (Called - 5)
> Queue : 4 1 2

Reference : 5
> Fault Occured
> Frequency : 1 (Called - 4) 2 (Called - 5) 5 (Called - 6)
> Queue : 1 2 5

Reference : 1
> Already Present
> Frequency : 1 (Called - 7) 2 (Called - 5) 5 (Called - 6)
> Queue : 1 2 5

Reference : 2
> Already Present
> Frequency : 1 (Called - 7) 2 (Called - 8) 5 (Called - 6)
> Queue : 1 2 5

Reference : 3
> Fault Occured
> Frequency : 1 (Called - 7) 2 (Called - 8) 3 (Called - 9)
> Queue : 1 2 3

Reference : 4
> Fault Occured
> Frequency : 2 (Called - 8) 3 (Called - 9) 4 (Called - 10)
> Queue : 2 3 4

Reference : 5
> Fault Occured
> Frequency : 3 (Called - 9) 4 (Called - 10) 5 (Called - 11)
> Queue : 3 4 5


Total Faults: 10


Goodbye !!!


Goodbye !!!

*/
