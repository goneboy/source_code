#ifndef HEADER_H_
#define HEADER_H_

typedef struct 
{
	unsigned short playerNum;
	unsigned short courtNum;
	unsigned short maxGame;
} BADMINTON;


BADMINTON Input(void);
unsigned short FindMaxGame(unsigned short P, unsigned short C);



#endif /* HEADER_H_ */