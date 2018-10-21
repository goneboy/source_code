#include "NumberOfDiscIntersection.h"

int main(int argc, char *argv[])
{
	int intersectCounter;
	DISC_ARR *discArr;

	// enter the disc array
	discArr = InputDiscArray();

	// find the intersection number
	intersectCounter = FindIntersectionNumber(discArr);

	// show the result
	printf("The intersection number is: %d\n", intersectCounter);

	// free the dynamic allocation
	delete[] discArr->disc;
	delete discArr;
	
	return 0;
}

