/*
An array A consisting of N integers is given. An inversion is a pair of indexes (P, Q) such that P < Q 
and A[Q] < A[P].

Write a function:

    int solution(int A[], int N);

that computes the number of inversions in A, or returns −1 if it exceeds 1,000,000,000.

Assume that:

        N is an integer within the range [0..100,000];
        each element of array A is an integer within the range [−2,147,483,648..2,147,483,647].

For example, in the following array:
 A[0] = -1 A[1] = 6 A[2] = 3
 A[3] =  4 A[4] = 7 A[5] = 4

there are four inversions:
   (1,2)  (1,3)  (1,5)  (4,5)

so the function should return 4.

Complexity:

        expected worst-case time complexity is O(N*log(N));
        expected worst-case space complexity is O(N), beyond input storage (not counting the storage 
        required for input arguments).
*/


#include "InversionCount.h"

INPUT_ARRAY* InputArray()
{
	INPUT_ARRAY* inputArr;
	inputArr = new INPUT_ARRAY;

	cout  << "Enter the element of the array: ";
	cin >> inputArr->arrElement;

	inputArr->numArr = new SINT [inputArr->arrElement];

	for(UINT i = 0; i < inputArr->arrElement; i++)
	{
		cout << "arr[" << i << "] = ";
		cin >> inputArr->numArr[i];
	}


	return inputArr;
}

UINT CalcInversionCount(SINT arrIn[], UINT arrElementNumIn, INVERSION_PAIR *inversionPairArr)
{
	UINT loopNum = arrElementNumIn - 1;
	UINT counter = 0;

	for(UINT i = 0; i < loopNum; i++)
	{
		for(UINT j = i + 1; j < arrElementNumIn; j++)
		{
			if(arrIn[i] > arrIn[j])
			{
				// store 2 inversion index
				inversionPairArr[counter].firstIndex = i;
				inversionPairArr[counter++].secondIndex = j;
			}
		}	

	}

	return counter;
}




