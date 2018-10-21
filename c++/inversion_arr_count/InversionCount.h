#ifndef INVERSION_COUNT_H_
#define INVERSION_COUNT_H_

#include <iostream>

using namespace std;

typedef char SCHAR;
typedef unsigned char UCHAR;
typedef int SINT;
typedef unsigned int UINT;
typedef void VOID;


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct
{
	UINT firstIndex;
	UINT secondIndex;
} INVERSION_PAIR;

typedef struct
{
	SINT *numArr;
	UINT arrElement;
} INPUT_ARRAY;



UINT CalcInversionCount(SINT arrIn[], UINT arrElementNumIn, INVERSION_PAIR *inversionPairArr);

INPUT_ARRAY* InputArray();

#endif /* INVERSION_COUNT_H_ */

