#ifndef NUMBER_OF_DISC_INTERSECTION_H_
#define NUMBER_OF_DISC_INTERSECTION_H_

#include <iostream>
#include <stdio.h>

typedef struct  
{
	unsigned int radius;
	int leftAbscissa;
	int rightAbscissa;
} DISC;

typedef struct  
{
	DISC *disc;
	unsigned int discNum;
} DISC_ARR;



DISC_ARR* InputDiscArray(void);
bool CheckIntersect(DISC fDisc, DISC sDisc);
int FindIntersectionNumber(DISC_ARR *discArr);

#endif /* NUMBER_OF_DISC_INTERSECTION_H_ */