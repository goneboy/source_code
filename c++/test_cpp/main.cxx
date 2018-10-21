#include <iostream>
#include "header.h"


using namespace std;


int main(int argc, char *argv[])
{
	cout << "Execute in main\n";
	
	return 0;
}


void StartUpFunc()
{
	cout << "Execute before main\n";

	//return;
}

void CleanUpFunc()
{
	cout << "Execute after main\n";

	//return;
}