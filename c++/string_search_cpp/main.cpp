#include "StringSearch.h"
#include <graphics.h>

int main(int argc, char *argv[])
{
	// declare and define source string
	const char *sourceString = InputSource();

	// declare and define sub string
	const char *subString = InputSubSource();

	if ((sourceString != NULL) && (subString != NULL))
	{
		const char *fFoundSub = StrSearch(sourceString, subString);

		if (fFoundSub != NULL)
		{
			cout << "Sub string is found at: " << fFoundSub;
		}
		else
		{
			cout << "The sub string is not found in source string\n";
		}

		// delete the dynamic allocation
		delete[] sourceString;
		delete[] subString;
	}
	
	return 0;
}


