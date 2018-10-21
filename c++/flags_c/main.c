/*
A non-empty zero-indexed array A consisting of N integers is given.

A peak is an array element which is larger than its neighbours.
More precisely, it is an index P such that 0 < P < N − 1 and A[P − 1] < A[P] > A[P + 1].

For example, the following array A:
    A[0] = 1
    A[1] = 5
    A[2] = 3
    A[3] = 4
    A[4] = 3
    A[5] = 4
    A[6] = 1
    A[7] = 2
    A[8] = 3
    A[9] = 4
    A[10] = 6
    A[11] = 2

has exactly four peaks: elements 1, 3, 5 and 10.

You are going on a trip to a range of mountains whose relative heights are
represented by array A, as shown in a figure below. You have to choose how
many flags you should take with you. The goal is to set the maximum number
of flags on the peaks, according to certain rules.

Flags can only be set on peaks. What's more, if you take K flags, then the
distance between any two flags should be greater than or equal to K. The
distance between indices P and Q is the absolute value |P − Q|.

For example, given the mountain range represented by array A, above, with N = 12, if you take:

        two flags, you can set them on peaks 1 and 5;
        three flags, you can set them on peaks 1, 5 and 10;
        four flags, you can set only three flags, on peaks 1, 5 and 10.

You can therefore set a maximum of three flags in this case.

Write a function:

    int solution(int A[], int N);

that, given a non-empty zero-indexed array A of N integers, returns the maximum number of flags that can be set on the peaks of the array.

For example, the following array A:
    A[0] = 1
    A[1] = 5
    A[2] = 3
    A[3] = 4
    A[4] = 3
    A[5] = 4
    A[6] = 1
    A[7] = 2
    A[8] = 3
    A[9] = 4
    A[10] = 6
    A[11] = 2

the function should return 3, as explained above.

Assume that:

        N is an integer within the range [1..400,000];
        each element of array A is an integer within the range [0..1,000,000,000].

Complexity:

        expected worst-case time complexity is O(N);
        expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).

Elements of input arrays can be modified.
 */

#include <stdio.h>
#include "flags.h"
#include <string.h>

int main(int argc, char *argv[])
{
	//int arr[] = {1, 5, 3, 4, 3, 4, 1, 2, 3, 4, 6, 2};
	int arr[32];
	int arr_element;
	int max_flag;
	//int arr_peak[] = {2, 6, 10, 12, 16};

	input_array(arr, &arr_element);
	max_flag = solution(arr, 12);

	printf("Max flags are: %d\n", max_flag);

	return 0;
}

//input the element of array
void input_array(int arr_out[], int *ptr_arr_element_out)
{
	printf("Enter the element of array: ");
	scanf("%d", ptr_arr_element_out);

	int i;
	for(i = 0; i < *ptr_arr_element_out; i++)
	{
		printf("arr[%d] = ", i);
		scanf("%d", &arr_out[i]);
	}

	return;
}

//the solution of the problem
int solution(int A[], int N)
{
	int arr_peak[32];
	int peak_element;
	int max_flag;

	//find the peaks of the mountains
	find_peaks(A, N, arr_peak, &peak_element);

	max_flag = find_max_flag(arr_peak, peak_element);

	return max_flag;
}

//find the peaks of the mountains
void find_peaks(int arr_in[], int arr_element_in, int arr_peak_out[], int *ptr_peak_element_out)
{
	int counter = 0;
	int i;

	for(i = 0; i < arr_element_in - 2; i++)
	{
		if((arr_in[i] < arr_in[i + 1]) && (arr_in[i + 1] > arr_in[i + 2]))
		{
			arr_peak_out[counter++] = i + 1;
		}
	}

	*ptr_peak_element_out = counter;

	return;
}

//find the max flags
int find_max_flag(int arr_peak_in[], int peak_element_in)
{
	int i;

	for(i = peak_element_in; i > 0; i--)
	{
		if(i == find_flag_num(arr_peak_in, peak_element_in, i))
		{
			return i;
		}
	}

	return 0;
}

//find the flags number
int find_flag_num(int arr_peak_in[], int peak_element_in, int flag_num_in)
{
	int i;
	int counter = 1;
	int tmp;
	int k = 0;

	for(i = 0; i < peak_element_in - 1; i++)
	{
		tmp = arr_peak_in[k];

		if(sub_abs(arr_peak_in[i + 1], tmp) >= flag_num_in)
		{
			k = i + 1;
			counter++;
		}
	}

	return counter;
}

//the absolute subtract between 2 numbers
int sub_abs(int a, int b)
{
	if(a > b)
	{
		return a - b;
	}
	else
	{
		return b - a;
	}
}



