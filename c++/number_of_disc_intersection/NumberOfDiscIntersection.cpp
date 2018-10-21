#include "NumberOfDiscIntersection.h"


DISC_ARR* InputDiscArray()
{
	DISC_ARR *discArr;
	discArr = new DISC_ARR;

	printf("Enter disc number: ");
	scanf("%u", &discArr->discNum);

	// allocate memory for disc array
	discArr->disc = new DISC[discArr->discNum];

	printf("Enter the radius of disc:\n");

	for (unsigned int i = 0; i < discArr->discNum; i++)
	{
		printf("disc[%u] = ", i);
		scanf("%u", &discArr->disc[i].radius);

		// calculate the left and right abscissa of every disc
		discArr->disc[i].leftAbscissa = i - discArr->disc[i].radius;
		discArr->disc[i].rightAbscissa = i + discArr->disc[i].radius;
	}

	return discArr;
}


int FindIntersectionNumber(DISC_ARR *discArr)
{
	unsigned int intersecCounter = 0;
	bool checkFlag;

	// loop to find the intersection number between two discs
	for (unsigned int i = 0; i < discArr->discNum - 1; i++)
	{
		for (unsigned int j = i + 1; j < discArr->discNum; j++)
		{
			checkFlag = CheckIntersect(discArr->disc[i], discArr->disc[j]);

			// if 2 discs are intersecting -> increase the intersection counter to 1 
			if (checkFlag)
			{
				intersecCounter++;
			}
		}
	}

	// return the intersection number
	return intersecCounter;
}

bool CheckIntersect(DISC fDisc, DISC sDisc)
{
	unsigned int counter = 0;

	// case [fDisc.leftAbscissa; sDisc.leftAbscissa; fDisc.rightAbscissa; sDisc.rightAbscissa]
	if (((fDisc.leftAbscissa <= sDisc.leftAbscissa) && (sDisc.leftAbscissa <= fDisc.rightAbscissa) && (fDisc.rightAbscissa <= sDisc.rightAbscissa)) || \
		// case [sDisc.leftAbscissa; fDisc.leftAbscissa; sDisc.rightAbscissa; fDisc.rightAbscissa]
		((sDisc.leftAbscissa <= fDisc.leftAbscissa) && (fDisc.leftAbscissa <= sDisc.rightAbscissa) && (sDisc.rightAbscissa <= fDisc.rightAbscissa)) || \
		// case [fDisc.leftAbscissa; sDisc.leftAbscissa; sDisc.rightAbscissa; fDisc.rightAbscissa]
		((fDisc.leftAbscissa <= sDisc.leftAbscissa) && (sDisc.leftAbscissa <= sDisc.rightAbscissa) && (sDisc.rightAbscissa <= fDisc.rightAbscissa)) || \
		// case [sDisc.leftAbscissa; fDisc.leftAbscissa; fDisc.rightAbscissa; sDisc.rightAbscissa]
		((sDisc.leftAbscissa <= fDisc.leftAbscissa) && (fDisc.leftAbscissa <= fDisc.rightAbscissa) && (fDisc.rightAbscissa <= sDisc.rightAbscissa)))
	{
		counter++;
	}

	// if counter equal 1 -> return true
	if (counter == 1)
	{
		return true;
	}

	// return false
	return false;
}

