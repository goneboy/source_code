/*************************************/
/* File: 	calcClockAngle.c  		 */
/* Author: 	Tran Bao Trung			 */
/* Purpose: Calculate the angle      */
/* between hour-hand and minute-hand */
/* Date: 	25-Nov-17 				 */
/************************************/

#include <stdio.h>
#include <string.h>
#include "calcClockAngle.h"

int a;
// main function
int main(int argc, char *argv[])
{
	int a = a;
//	TIME time;
//
//	time = GetTime();
//
//	U_SHORT time_angle = CalcTimeAngle(time);
//
//	printf("The angle between hour and minute is: %u degrees\n", time_angle);

	return 0;
}

/****************************************/
/* Function: 	GetTime 				*/
/* Purpose:		get the hour and minute */
/* Arguments:	void					*/
/* Return:		TIME					*/
/****************************************/
TIME GetTime(void)
{
	TIME tmp;

	// set elements of struct is 0
	memset(&tmp, 0, sizeof(TIME));

	// we choose 12-hour format
	printf("Enter hour: ");
	scanf("%u", &tmp.hour);

	// if the hour is out of 11, re-enter
	while (tmp.hour > 11)
	{
		printf("The hour must be 12-hour format. Enter again: ");
		scanf("%u", &tmp.hour);
	}

	// if the minute is out of 59, re-enter
	printf("Enter minute: ");
	scanf("%u", &tmp.minute);

	while (tmp.minute > 59)
	{
		printf("The minute must be around 0 to 59. Enter again: ");
		scanf("%u", &tmp.minute);
	}

	return tmp;
}

/****************************************/
/* Function: 	CalcTimeAngle 			*/
/* Purpose:		calculate the angle 	*/
/* between hour and minute following	*/
/* clock direction						*/
/* Arguments:	TIME					*/
/* Return:		U_SHORT					*/
/****************************************/
U_SHORT CalcTimeAngle(TIME time_in)
{
	// declare angle between hour and minute
	U_SHORT angle;

	// calculate the position of hour following minute
	U_CHAR hour_pos = time_in.hour * 5;

	// if the minute is greater than or equal the hour position
	if (time_in.minute >= hour_pos)
	{
		angle = ((time_in.minute - hour_pos) * ANGLE_MUNITE_UNIT) - CalcHourHand(time_in.minute);
	}
	// if the minute is less than the hour position
	else
	{
		angle = ((hour_pos - time_in.minute)  * ANGLE_MUNITE_UNIT) + CalcHourHand(time_in.minute);
	}

	return angle;
}

/****************************************/
/* Function: 	CalcHourHand 			*/
/* Purpose:		calculate the angle 	*/
/* that hour-hand goes to, depending on	*/
/* the minute-hand						*/
/* Arguments:	U_INT					*/
/* Return:		U_SHORT					*/
/****************************************/
U_SHORT CalcHourHand(U_INT minute_in)
{
	// every 12 minutes --> hour-hand goes 6 degrees
	// therefore minute_in minutes --> hour-hand will go:
	U_SHORT tmp = (minute_in * ANGLE_MUNITE_UNIT) / HOUR_UNIT;  // degrees

	return tmp;
}

