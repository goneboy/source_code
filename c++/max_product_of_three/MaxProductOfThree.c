/*
A non-empty array A consisting of N integers is given. The product of triplet (P, Q, R) 
equates to A[P] * A[Q] * A[R] (0 ≤ P < Q < R < N).

For example, array A such that:
A[0] = -3
A[1] = 1
A[2] = 2
A[3] = -2
A[4] = 5
A[5] = 6

contains the following example triplets:

(0, 1, 2), product is −3 * 1 * 2 = −6
(1, 2, 4), product is 1 * 2 * 5 = 10
(2, 4, 5), product is 2 * 5 * 6 = 60

Your goal is to find the maximal product of any triplet.

Write a function:

int solution(int A[], int N);

that, given a non-empty array A, returns the value of the maximal product of any triplet.

For example, given array A such that:
A[0] = -3
A[1] = 1
A[2] = 2
A[3] = -2
A[4] = 5
A[5] = 6

the function should return 60, as the product of triplet (2, 4, 5) is maximal.

Assume that:

N is an integer within the range [3..100,000];
each element of array A is an integer within the range [−1,000..1,000].

Complexity:

expected worst-case time complexity is O(N*log(N));
expected worst-case space complexity is O(1), beyond input storage (not counting the storage 
required for input arguments).
*/
#define _CRT_SECURE_NO_WARNINGS

#include "MaxProductOfThree.h"

ARRAY* InputArray()
{
	ARRAY *arr;
	arr = (ARRAY *) calloc(1, sizeof(ARRAY));

	printf("Enter the element number of array: ");
	scanf("%d", &arr->arrElementNum);

	while ((arr->arrElementNum < MIN_ELEMENT) || (arr->arrElementNum > MAX_VALUE))
	{
		printf("The element must in [%d; %d]\n", MIN_ELEMENT, MAX_VALUE);
		printf("Enter the element number of array: ");
		scanf("%d", &arr->arrElementNum);
	}

	arr->arrValue = (short *)calloc(arr->arrElementNum, sizeof(short));

	printf("Enter the element of array:\n");

	for (int i = 0; i < arr->arrElementNum; i++)
	{
		printf("arr[%d] = ", i);
		scanf("%hi", &arr->arrValue[i]);

		// check the value of elements in [-1000; 1000]
		while ((arr->arrValue[i] < MIN_VALUE) || (arr->arrValue[i] > MAX_VALUE))
		{
			printf("The value of elements must in [%d; %d]\n", MIN_VALUE, MAX_VALUE);
			printf("arr[%d] = ", i);
			scanf("%hi", &arr->arrValue[i]);
		}
	}

	return arr;
}

PRODUCT* FindMaxProduct(ARRAY *arr)
{
	PRODUCT *pro;
	pro = (PRODUCT *)malloc(sizeof(PRODUCT));
	pro->maxProduct = 0;
	pro->elementIndex = (int *)calloc(3, sizeof(int));
	pro->counter = 0;

	for (int i = 0; i < arr->arrElementNum - 2; i++)
	{
		for (int j = i + 1; j < arr->arrElementNum - 1; j++)
		{
			for (int k = j + 1; k < arr->arrElementNum; k++)
			{
				int tmpProduct = FindProductOfThree(arr->arrValue[i], arr->arrValue[j], arr->arrValue[k]);

				if (tmpProduct > pro->maxProduct)
				{
					pro->maxProduct = tmpProduct;
				}
			}
		}
	}

	// find the index of three elements that have max product
	for (int i = 0; i < arr->arrElementNum - 2; i++)
	{
		for (int j = i + 1; j < arr->arrElementNum - 1; j++)
		{
			for (int k = j + 1; k < arr->arrElementNum; k++)
			{
				int tmpProduct = FindProductOfThree(arr->arrValue[i], arr->arrValue[j], arr->arrValue[k]);

				if (tmpProduct == pro->maxProduct)
				{
					pro->elementIndex[pro->counter++] = i;
					pro->elementIndex[pro->counter++] = j;
					pro->elementIndex[pro->counter++] = k;
				}
			}
		}
	}

	return pro;
}

int FindProductOfThree(short firstArg, short secondArg, short thirdArg)
{
	return (int) (firstArg * secondArg * thirdArg);
}

