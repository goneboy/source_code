#ifndef CALC_CLOCK_ANGLE_H_
#define CALC_CLOCK_ANGLE_H_

// 60 minutes = 360 degrees --> 1 minute = 6 degrees
#define ANGLE_MUNITE_UNIT (6)

// when minute-hand goes 12 minutes --> hour-hand will go 6 degrees
#define HOUR_UNIT (12)

typedef unsigned int U_INT;
typedef int S_INT;
typedef unsigned char U_CHAR;
typedef char S_CHAR;
typedef unsigned short U_SHORT;
typedef short S_SHORT;

typedef struct
{
	U_INT hour;
	U_INT minute;
} TIME;


// function prototypes
TIME GetTime(void);
U_SHORT CalcTimeAngle(TIME time_in);
U_SHORT CalcHourHand(U_INT minute_in);


#endif
