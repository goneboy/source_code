#ifndef STAFF_MANAGEMENT_H_
#define STAFF_MANAGEMENT_H_

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

class Staff
{
private:
	unsigned int sID;
	string sName;
public:
	Staff();
	void Input(void);
	void Output(void);
};


class ProductionStaff : public Staff
{
private:
	unsigned int pNum;
public:
	ProductionStaff();
	void Input(void);
	void Output(void);
};


#endif /* STAFF_MANAGEMENT_H_ */
