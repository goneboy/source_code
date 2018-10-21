#include "MaxProductOfThree.h"

int main(int argc, char *argv[])
{
	// declare pointer has ARRAY data type
	ARRAY *arr;

	// get the result from InputArray()
	arr = InputArray();

	// declare pointer has PRODUCT data type
	PRODUCT *pro;

	// get the result from FindMaxProduct()
	pro = FindMaxProduct(arr);

	printf("Max product: %d\n", pro->maxProduct);
	printf("Index of three elements: ");
	
	printf("[");
	for (int i = 0; i < pro->counter; i++)
	{
		if (i != pro->counter - 1)
		{
			printf("%d, ", pro->elementIndex[i]);
		}
		else
		{
			printf("%d]\n", pro->elementIndex[i]);
		}
	}

	// free the dynamic allocation memory
	free(arr->arrValue);
	free(arr);
	free(pro->elementIndex);
	free(pro);

	return 0;
}


