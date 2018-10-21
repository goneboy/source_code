#include "StringSearch.h"

// Function: const char* StrSearch(const char* sourceString, const char* subString)
// Parameter:
// substr: string to be searched for
// source: string in which 'substr' is searched
// Return: pointer to location where 'substr' is first found inside 'source'
// NULL if not found

const char* StrSearch(const char *sourceString, const char *subString)
{
	// get source string length
	unsigned int sLen = strlen(sourceString) - 1;

	// get sub source string length
	unsigned int subLen = strlen(subString) - 1;

	// the counter to count the matching character between source string and sub source string
	unsigned int counter = 0;

	// for loop to scan the source string
	for (unsigned int i = 0; i < sLen; i++)
	{
		// if the character i of source string equals the first character of sub string,
		// then, we will scan the sub source string, count the matching characters
		// between source string and sub string. If the matching characters are equal
		// the sub string length, return the address of the first character of matching string
		// otherwise return NULL
		if (sourceString[i] == subString[0])
		{
			// for loop to scan the sub source string
			for (unsigned int j = 0; j < subLen; j++)
			{
				// if sourceString[j + i] is match with subString[j], increase the counter to 1
				if (sourceString[j + i] == subString[j])
				{
					counter++;
				}
				// if not, break the loop
				else
				{
					break;
				}
			}

			// if the sub string is match with first sub string of source string, return the address of first matching character
			if (counter == subLen)
			{
				return &sourceString[i];
			}
			// if not, reset the counter
			else
			{
				counter = 0;
			}
		}
	}

	return NULL;
}

// Function: const char* InputSource(void)
// Parameter: void
// Return: pointer to source string 
// NULL if can not allocate

const char* InputSource(void)
{
	const char *sourceString;

	// the max length of source string is 256
	sourceString = new char[256];

	// check the string can be allocated
	if (sourceString == NULL)
	{
		cout << "String's allocation error\n";

		return sourceString;
	}

	cout << "Enter the source string: ";

	fgets((char*) sourceString, 256, stdin);

	return sourceString;
}


// Function: const char* InputSubSource(void)
// Parameter: void
// Return: pointer to sub source string 
// NULL if can not allocate

const char* InputSubSource(void)
{
	const char *subString;

	// the max length of sub source string is 256
	subString = new char[256];

	// check the string can be allocated
	if (subString == NULL)
	{
		cout << "String's allocation error\n";

		return subString;
	}

	cout << "Enter the sub string: ";

	fgets((char*) subString, 256, stdin);

	return subString;
}

