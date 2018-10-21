/****************************************/
/* Author: Tran Bao Trung				*/
/* Date: 17-Jul-17						*/
/****************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//define struct of integer distances
typedef struct
{
	int head; //head of integer integer
	int tail; //tail of integer integer
	int pos_num_distance; //number of positive distance
	int neg_num_distance; //number of negative distance which has negative integer
} INT_DISTANCE;

//define global struct
INT_DISTANCE int_distance;
INT_DISTANCE int_distance_arr[1000];

//global counter variable
int counter = 0;

//define MACRO
#define NEGATIVE_PERCENT 30
#define TOTAL_PERCENT 100

//function prototypes
int random_pos_num(void);
void problem_1(void);
void problem_2(void);
void sort_array(INT_DISTANCE arr_in[], int arr_size_in);
bool check_occur_in_array(int a, int b, int c, int d);
int distance_size(int head, int tail);
void sort_array_size(INT_DISTANCE arr_in[], int arr_size_in);


//main function
int main(int argc, char *argv[])
{
	//the solution for problem 1
	problem_1();

	//the solution for problem 2
	problem_2();

	return 0;
}

//the solution for problem 1
void problem_1(void)
{
	srand((unsigned int)time(NULL));
	FILE *write_file;

	//open file to write
	write_file = fopen("output1.txt", "w+");

	int i;
	int N = random_pos_num();
	//int N = 5;

	//calculate negative number distances and positive number distance
	int_distance.neg_num_distance = (N * NEGATIVE_PERCENT) / TOTAL_PERCENT;
	int_distance.pos_num_distance = N - int_distance.neg_num_distance;

	//write data to output file
	fprintf(write_file, "%d\n", N);

	//generate negative number of distances (-99 - 99)
	for (i = 0; i < int_distance.neg_num_distance; i++)
	{
		int a = (rand() % (200)) + (-99);
		int b;

		if (a < 0)
		{
			b = (rand() % (200)) + (-99);
		}
		else
		{
			b = (rand() % (99)) + (-99);
		}

		//write data to output file
		fputs("(", write_file);

		//head is always less than or equal tail
		if (a >= b)
		{
			int_distance.head = b;
			int_distance.tail = a;
		}
		else
		{
			int_distance.head = a;
			int_distance.tail = b;
		}

		//write data to output file
		fprintf(write_file, "%d", int_distance.head);
		fputs(";", write_file);
		fprintf(write_file, "%d", int_distance.tail);
		fputs(")\n", write_file);

		int_distance_arr[counter].head = int_distance.head;
		int_distance_arr[counter].tail = int_distance.tail;

		//increase the counter
		counter++;
	}

	//generate positive number of distances (0 - 99)
	for (i = 0; i < int_distance.pos_num_distance; i++)
	{
		int a = (rand() % (100)) + (0);
		int b = (rand() % (100)) + (0);

		//write data to output file
		fputs("(", write_file);

		//head is always less than or equal tail
		if (a >= b)
		{
			int_distance.head = b;
			int_distance.tail = a;
		}
		else
		{
			int_distance.head = a;
			int_distance.tail = b;
		}

		//write data to output file
		fprintf(write_file, "%d", int_distance.head);
		fputs(";", write_file);
		fprintf(write_file, "%d", int_distance.tail);
		fputs(")\n", write_file);

		int_distance_arr[counter].head = int_distance.head;
		int_distance_arr[counter].tail = int_distance.tail;
		counter++;
	}

	return;
}

//the solution for problem 2
void problem_2()
{
	FILE *write_file;

	//open file to write
	write_file = fopen("output2.txt", "w+");

	int i;
	int k = 0;
	INT_DISTANCE tmp_distance;
	INT_DISTANCE tmp_distance_arr[1000];

	//sort the array firstly follow tail
	sort_array(int_distance_arr, counter);

	//store the first element of array
	tmp_distance = int_distance_arr[0];
	tmp_distance_arr[k++] = tmp_distance;

	//loop for check the next element
	for (i = 1; i < counter; i++)
	{
		if (check_occur_in_array(tmp_distance.head, tmp_distance.tail, int_distance_arr[i].head, int_distance_arr[i].tail))
		{
			tmp_distance = int_distance_arr[i];
			tmp_distance_arr[k++] = tmp_distance;
		}
	}

	//sort array follow size of distance
	//sort_array_size(tmp_distance_arr, k);

	//write data to the file
	for (i = 0; i < k; i++)
	{
		fputs("(", write_file);
		fprintf(write_file, "%d", tmp_distance_arr[i].head);
		fputs(";", write_file);
		fprintf(write_file, "%d", tmp_distance_arr[i].tail);
		fputs(")\n", write_file);
	}

	return;
}

//random positive number from 11 to 999
int random_pos_num()
{
	srand((unsigned int) time(NULL));
	return (rand() % (989)) + (11);
}

//sort the array order ascending
void sort_array(INT_DISTANCE arr_in[], int arr_size_in)
{
	int i;
	int j;

	for (i = 0; i < arr_size_in - 1; i++)
	{
		for (j = 0; j < arr_size_in - 1 - i; j++)
		{
			if (arr_in[j].tail > arr_in[j + 1].tail)
			{
				INT_DISTANCE tmp = arr_in[j];
				arr_in[j] = arr_in[j + 1];
				arr_in[j + 1] = tmp;
			}
		}
	}

	return;
}

//check a number occurs in array or not
bool check_occur_in_array(int a, int b, int c, int d)
{
	int i;

	for (i = c; i <= d; i++)
	{
		if ((a == i) || (b == i))
		{
			return false;
		}
	}

	return true;
}

//sort array follow size of distance
void sort_array_size(INT_DISTANCE arr_in[], int arr_size_in)
{
	int i;
	int j;

	for (i = 0; i < arr_size_in - 1; i++)
	{
		for (j = 0; j < arr_size_in - 1 - i; j++)
		{
			if (distance_size(arr_in[j].head, arr_in[j].tail) > distance_size(arr_in[j + 1].head, arr_in[j + 1].tail))
			{
				INT_DISTANCE tmp = arr_in[j];
				arr_in[j] = arr_in[j + 1];
				arr_in[j + 1] = tmp;
			}
		}
	}

	return;
}

//calculate the distance's size
int distance_size(int head, int tail)
{
	return tail - head - 1;
}
