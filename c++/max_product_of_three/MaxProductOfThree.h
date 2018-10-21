#ifndef MAX_PRODUCT_OF_THREE
#define MAX_PRODUCT_OF_THREE

#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENT	100000
#define MIN_ELEMENT 3
#define MAX_VALUE	1000
#define MIN_VALUE	-1000

typedef struct  
{
	short *arrValue;
	int arrElementNum;
} ARRAY;

typedef struct
{
	int maxProduct;
	int counter;
	int *elementIndex;
} PRODUCT;


ARRAY* InputArray(void);
int FindProductOfThree(short firstArg, short secondArg, short thirdArg);
PRODUCT* FindMaxProduct(ARRAY *arr);


#endif /* MAX_PRODUCT_OF_THREE */