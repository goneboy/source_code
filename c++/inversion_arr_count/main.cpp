#include <iostream>
#include "InversionCount.h"

SINT main(SINT argc, SCHAR *argv[])
{	
	INPUT_ARRAY *inputArr;
	INVERSION_PAIR *invPairArr;

	inputArr = InputArray();
	invPairArr = new INVERSION_PAIR [inputArr->arrElement];

	UINT invCount = CalcInversionCount(inputArr->numArr, inputArr->arrElement, invPairArr);

	cout << "Inversion counts are: " << invCount << endl;

	for(UINT i = 0; i < invCount; i++)
	{
		cout << invPairArr[i].firstIndex << ", ";
		cout << invPairArr[i].secondIndex << endl;
	}

	// free the memory
	delete [] invPairArr;
	delete [] inputArr->numArr;
	delete inputArr;

	return 0;
}


