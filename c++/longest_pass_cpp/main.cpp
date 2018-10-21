/*
You would like to set a password for a bank account. 
However, there are three restrictions on the format of the password:

it has to contain only alphanumerical characters (a−z, A−Z, 0−9);
there should be an even number of letters;
there should be an odd number of digits.

You are given a string S consisting of N characters. String S can be 
divided into words by splitting it at, and removing, the spaces. The 
goal is to choose the longest word that is a valid password. You can 
assume that if there are K spaces in string S then there are exactly K + 1 words.

For example, given "test 5 a0A pass007 ?xy1", there are five words 
and three of them are valid passwords: "5", "a0A" and "pass007". Thus 
the longest password is "pass007" and its length is 7. Note that neither 
"test" nor "?xy1" is a valid password, because "?" is not an alphanumerical 
character and "test" contains an even number of digits (zero).

Write a function:

int solution(char *S);

that, given a non-empty string S consisting of N characters, returns the 
ength of the longest word from the string that is a valid password. If there 
is no such word, your function should return −1.

For example, given S = "test 5 a0A pass007 ?xy1", your function should return 7, as explained above.

Assume that:

N is an integer within the range [1..200];
string S consists only of printable ASCII characters and spaces.

In your solution, focus on correctness. The performance of your solution will not be the focus of the assessment.

*/

#include <iostream>
#include "longest_pass.h"

using namespace std;

S_INT main(S_INT argc, S_CHAR *argv[])
{

	S_CHAR *str;
	str = Input();
	
	S_INT pass_max_num = solution(str);

	if (pass_max_num != -1)
	{
		cout << "Max string of password: " << pass_max_num << endl;
	}
	else
	{
		cout << "There no password matching\n";
	}
	

	delete[] str;

	return 0;
}

S_INT solution(S_CHAR *S)
{
	U_CHAR pass_num = CalcSpaceNum(S) + 1;
	S_INT max_num = 0;

	PASSWORD *pass;

	// get structure password array
	pass = ExtractPass(S);

	for (U_CHAR i = 0; i < pass_num; i++)
	{
		if (CheckAlphanumerical(pass[i]) && CheckEvenLetterNum(pass[i]) && CheckOddDigitNum(pass[i]))
		{
			if (pass[i].element > max_num)
			{
				max_num = pass[i].element;
			}
		}
	}

	// if there no password matching -> return -1
	if (max_num == 0)
	{
		max_num = -1;
	}

	// delete allocated memory
	for(U_CHAR i = 0; i < pass_num; i++)
	{
		delete [] pass[i].pass;
	}

	delete[] pass;

	return max_num;
}


S_CHAR* Input()
{
	S_CHAR *pass_str;
	pass_str = new S_CHAR [MAX_STRING];

	cout << "Enter password string: ";
	fgets(pass_str, MAX_STRING, stdin);

	return pass_str;
}

PASSWORD* ExtractPass(S_CHAR *str_in)
{
	// get the space number
	U_CHAR space_num = CalcSpaceNum(str_in);

	// declare string length
	U_CHAR str_len = static_cast<U_CHAR>(strlen(str_in));

	// declare counter for password number
	U_CHAR counter = 0;

	U_CHAR pass_num = space_num + 1;

	PASSWORD *pass;
	pass = new PASSWORD[pass_num];

	for (U_CHAR i = 0; i < pass_num; i++)
	{
		// set elements of struct pass to 0
		memset(&pass[i], 0, sizeof(PASSWORD));

		// allocate for string pass
		pass[i].pass = new S_CHAR[MAX_STRING];
	}

	for (U_CHAR i = 0; i < str_len; i++)
	{
		// if character is not space ' ' -> store to password array
		if (str_in[i] != ' ')
		{
			pass[counter].pass[pass[counter].element++] = str_in[i];
		}
		else
		{
			// increase the counter
			counter++;
		}
	}

	return pass;
}

U_CHAR CalcSpaceNum(S_CHAR *str_in)
{
	U_CHAR counter = 0;
	U_CHAR str_len = static_cast<U_CHAR>(strlen(str_in));

	for (U_CHAR i = 0; i < str_len; i++)
	{
		// if character is space ' ', increase the counter
		if (str_in[i] == ' ')
		{
			counter++;
		}
	}

	// return the counter value
	return counter;
}

BOOL CheckAlphanumerical(PASSWORD pass_in)
{
	BOOL flag = TRUE;

	for (U_CHAR i = 0; i < pass_in.element; i++)
	{
		if (((pass_in.pass[i] < 'a') || (pass_in.pass[i] > 'z')) && \
			((pass_in.pass[i] < 'A') || (pass_in.pass[i] > 'Z')) && \
			((pass_in.pass[i] < '0') || (pass_in.pass[i] > '9')))
		{
			flag = FALSE;
		}
	}
	
	return flag;
}

BOOL CheckEvenLetterNum(PASSWORD pass_in)
{
	BOOL flag = FALSE;
	U_CHAR counter = 0;

	for (U_CHAR i = 0; i < pass_in.element; i++)
	{
		// if element is character -> increase counter
		if ((pass_in.pass[i] >= 'a' && pass_in.pass[i] <= 'z') || \
			(pass_in.pass[i] >= 'A' && pass_in.pass[i] <= 'Z'))
		{
			counter++;
		}
	}

	// if counter is even -> set flag to TRUE
	if (counter % 2 == 0)
	{
		flag = TRUE;
	}

	return flag;
}

BOOL CheckOddDigitNum(PASSWORD pass_in)
{
	BOOL flag = FALSE;
	U_CHAR counter = 0;

	for (U_CHAR i = 0; i < pass_in.element; i++)
	{
		// if element is digit -> increase counter
		if ((pass_in.pass[i] >= '0') && (pass_in.pass[i] <= '9'))
		{
			counter++;
		}
	}

	// if counter is odd -> set flag to TRUE
	if (counter % 2 == 1)
	{
		flag = TRUE;
	}

	return flag;
}
