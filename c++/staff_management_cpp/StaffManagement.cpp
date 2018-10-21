#include "StaffManagement.h"

Staff::Staff()
{
	sID = 0;
	sName = "";
}

void Staff::Input(void)
{
	char c;
	cout << "Enter staff's ID: ";
	cin >> sID;

	// clear the buffer
	// fflush(stdin);
	while(((c = getchar()) == '\n') && (c == EOF));

	cout << "Enter staff's Name: ";
	getline(cin, sName);

	return;
}

void Staff::Output(void)
{
	cout << "Staff's ID: " << sID << endl;
	cout << "Staff's Name: " << sName << endl;

	return;
}