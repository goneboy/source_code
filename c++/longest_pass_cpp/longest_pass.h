#ifndef LONGEST_PASS_H_
#define LONGEST_PASS_H_

#include <string.h>
#include <stdio.h>

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#define MAX_STRING (200)

typedef unsigned char U_CHAR;
typedef char S_CHAR;
typedef unsigned int U_INT;
typedef int S_INT;
typedef void VOID;
typedef unsigned char BOOL;

typedef struct
{
	S_CHAR *pass;
	U_CHAR element;
} PASSWORD;


// function prototypes
S_CHAR* Input(void);
U_CHAR CalcSpaceNum(S_CHAR *pass_in);
PASSWORD* ExtractPass(S_CHAR *str_in);
BOOL CheckAlphanumerical(PASSWORD pass_in);
BOOL CheckEvenLetterNum(PASSWORD pass_in);
BOOL CheckOddDigitNum(PASSWORD pass_in);
S_INT solution(S_CHAR *S);


#endif /* LONGEST_PASS_H_ */

