/*
You are hosting a tennis tournament. P players, who will take part in the first round of this tournament, 
are already registered and you have reserved C tennis courts for the matches. Exactly two players play in 
each game and only one game can be played on each court at any given time. You want to host the maximum 
possible number of games starting at the same time (in order to finish the first round quickly).

How many games can be hosted in parallel simultaneously?

Write a function:

    int solution(int P, int C);

that, given the number of players P and the number of reserved courts C, returns the maximum number of 
games that can be played in parallel.

For example, given P = 5 players and C = 3 available courts, the function should return 2, as two games 
can be played simultaneously (for instance, the first and second players can play on the first court, and 
the third and fourth players on the second court, and the third court will be empty because the fifth 
player doesn't have a partner to play with).

Given P = 10 players and C = 3 courts, the function should return 3, as at most three games can be hosted 
in parallel.

Assume that:

        P and C are integers within the range [1..30,000].

In your solution, focus on correctness. The performance of your solution will not be the focus of the 
assessment.
*/

#include <stdio.h>
#include "header.h"

int main(int argc, char *argv[])
{
	BADMINTON badGame;

	badGame = Input();

	badGame.maxGame = FindMaxGame(badGame.playerNum, badGame.courtNum);

	printf("The max game is: %hu\n", badGame.maxGame);

	return 0;
}

BADMINTON Input(void)
{
	BADMINTON bad;

	printf("Enter the player number: ");
	scanf("%hu", &bad.playerNum);
	printf("Enter the court number: ");
	scanf("%hu", &bad.courtNum);

	return bad;
}

unsigned short FindMaxGame(unsigned short playerNum, unsigned short courtNum)
{
	unsigned short playerPair;
	unsigned short maxGame;

	playerPair = playerNum / 2;

	if(playerPair >= courtNum)
	{
		maxGame = courtNum;
	}
	else
	{
		maxGame = playerPair;
	}

	return maxGame;
}


