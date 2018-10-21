#include <stdio.h>
#include <string.h>
#include "oasisHandle.h"

U_INT CalcOasisLength(S_CHAR file_name_in[])
{
	FILE *read_file;
	U_INT i = 0;

	// open file to read
	read_file = fopen(file_name_in, "r");

	S_INT ch = getc(read_file);

	while(ch != EOF)
	{
		i++;
		ch = getc(read_file);
	}

	// check end of file or not
	if(feof(read_file))
	{
		printf("Reached end of file!\n");
	}
	else
	{
		printf("Something went wrong!\n");
	}

	// close the file
	fclose(read_file);

	return i;
}

VOID GetOasis2Array(S_INT oasis_arr[], U_INT &arr_element_out, const S_CHAR file_name_in[])
{
	FILE *read_file;
	U_INT i = 0;
	read_file = fopen(file_name_in, "r");

	if(read_file == NULL)
	{
		printf("Can not open the oasis file \"%s\"!\n", file_name_in);
		return;
	}

	S_INT ch = getc(read_file);

	while(ch != EOF)
	{
		oasis_arr[i++] = ch;
		ch = getc(read_file);
	}

	// check end of file or not
	if(feof(read_file))
	{
		printf("Reached end of file!\n");
	}
	else
	{
		printf("Something went wrong!\n");
	}

	arr_element_out = i;

	// close the file
	fclose(read_file);

	return;
}


U_INT Bin2Dec(U_CHAR arr[], U_SHORT arr_element_in)
{
	U_INT tmp = 0;

	for(S_SHORT i = 0; i < arr_element_in; i++)
	{
		tmp += arr[i] * (U_INT) Exponent2(arr_element_in - i - 1);
//		tmp += arr[i] << (arr_element_in - 1 - i);
	}

	return tmp;
}


DOUBLE Exponent2(S_SHORT exponent_in)
{
	DOUBLE tmp = 1;

	if(exponent_in != 0)
	{
		while(exponent_in > 0)
		{
			tmp *= 2;
			exponent_in--;
		}
	}

	if(exponent_in < 0)
	{
		while(exponent_in < 0)
		{
			tmp *= 2;
			exponent_in++;
		}

		tmp = (DOUBLE) 1 / tmp;
	}

	return tmp;
}


U_INT GetOasisUINT(U_INT byte_num_in, S_INT arr_in[], U_INT pos_in)
{
	U_CHAR buffer[SCHAR_MAX];
	U_SHORT bin_arr_element = 0;

	for(U_INT i = 0; i < byte_num_in; i++)
	{
		Bin7ToArray(arr_in[pos_in + i], buffer, bin_arr_element);
	}

	InvertArray(buffer, bin_arr_element);

	U_INT tmp = Bin2Dec(buffer, bin_arr_element);

	return tmp;
}


VOID Bin7ToArray(U_INT dec_in, U_CHAR arr[], U_SHORT &arr_element_out)
{
	if(dec_in >= 128)
	{
		while(dec_in / 2 != 0)
		{
			arr[arr_element_out++] = dec_in % 2;
			dec_in /= 2;
		}
	}
	else if (dec_in > 1 && dec_in < 128)
	{
		while(dec_in / 2 != 0)
		{
			arr[arr_element_out++] = dec_in % 2;
			dec_in /= 2;

			if(dec_in / 2 == 0)
			{
				arr[arr_element_out++] = 1;
			}
		}

		U_SHORT i = arr_element_out / 7;

		if(arr_element_out % 7 != 0)
		{
			while(arr_element_out < 7 * (i + 1))
			{
				arr[arr_element_out++] = 0;
			}
		}
	}
	else if (dec_in == 1)
	{
		U_SHORT i = arr_element_out;

		while(i < arr_element_out + 7)
		{
			if(i == arr_element_out)
			{
				arr[i++] = 1;
			}
			else
			{
				arr[i++] = 0;
			}
		}

		arr_element_out = i;
	}
	else
	{
		U_SHORT i = arr_element_out;

		while(i < arr_element_out + 7)
		{
			arr[i++] = 0;
		}

		arr_element_out = i;
	}

	return;
}


VOID Bin8ToArray(U_INT dec_in, U_CHAR arr[], U_SHORT &arr_element_out)
{
	if(dec_in >=2)
	{
		while(dec_in / 2 != 0)
		{
			arr[arr_element_out++] = dec_in % 2;
			dec_in /= 2;

			if(dec_in / 2 == 0)
			{
				arr[arr_element_out++] = 1;
			}
		}

		U_SHORT i = arr_element_out / 8;

		if(arr_element_out % 8 != 0)
		{
			while(arr_element_out < 8 * (i + 1))
			{
				arr[arr_element_out++] = 0;
			}
		}
	}
	else if (dec_in == 1)
	{
		U_SHORT i = arr_element_out;

		while(i < arr_element_out + 8)
		{
			if(i == arr_element_out)
			{
				arr[i++] = 1;
			}
			else
			{
				arr[i++] = 0;
			}
		}

		arr_element_out = i;
	}
	else
	{
		Bin8For0ToArray(arr, arr_element_out);
	}

	return;
}

VOID Bin8For0ToArray(U_CHAR arr[], U_SHORT &arr_element_out)
{
	U_SHORT i = arr_element_out;

	while(i < arr_element_out + 8)
	{
		arr[i++] = 0;
	}

	arr_element_out = i;

	return;
}

DOUBLE GetIeee8(const U_CHAR arr[])
{
	U_CHAR sign = arr[0];
	U_SHORT exponent = 0;
	DOUBLE fraction = 1;
	DOUBLE tmp;

	for(U_CHAR i = 1; i <= 11; i++)
	{
		exponent += arr[i] * (U_SHORT) Exponent2(11 - i);
	}

	for(U_CHAR i = 12; i < 63; i++)
	{
		fraction += (arr[i] * Exponent2((-1) * (i - 11)));
	}

	if(sign == 0)
	{
		tmp = Exponent2(exponent - 1023) * fraction;
	}
	else
	{
		tmp = (-1) * Exponent2(exponent - 1023) * fraction;
	}

	return tmp;
}


VOID InvertArray(U_CHAR arr[], U_CHAR arr_element_in)
{
	U_CHAR arr_tmp[ARRAY_MAX];

	for(U_CHAR i = 0; i < arr_element_in; i++)
	{
		arr_tmp[i] = arr[arr_element_in - i - 1];
	}

	memcpy(arr, arr_tmp, arr_element_in);

	return;
}


DOUBLE GetUnit(S_INT arr_oasis[], U_INT unit_start_pos_in, U_CHAR &unit_length_out)
{
	U_CHAR buffer[128];
	U_SHORT arr_element = 0;
	DOUBLE unit_value = 0.0;

	switch(arr_oasis[unit_start_pos_in])
	{
		//ieee-8
		case 7:
			for(U_CHAR i = 1; i <= 8; i++)
			{
				Bin8ToArray(arr_oasis[unit_start_pos_in + i], buffer, arr_element);
			}

			InvertArray(buffer, arr_element);
			unit_value = GetIeee8(buffer);
			unit_length_out = 8 + 1;
			break;

		default:
			printf("Do not define unit type yet\n");
			break;
	}

	return unit_value;
}


U_INT GetIntegerByteNum(S_INT oasis_arr[], U_INT integer_pos_in)
{
	U_INT counter = 1;

	while(oasis_arr[integer_pos_in++] >= 128)
	{
		counter++;
	}

	return counter;
}



S_INT GetOasisSINT(U_INT byte_num_in, S_INT arr_in[], U_INT pos_in)
{
	U_CHAR buffer[SCHAR_MAX];
	U_SHORT bin_arr_element = 0;

	for(U_INT i = 0; i < byte_num_in; i++)
	{
		Bin7ToArray(arr_in[pos_in + i], buffer, bin_arr_element);
	}

	InvertArray(buffer, bin_arr_element);

	// get the sign of integer
	BOOL sign = buffer[bin_arr_element - 1];
	S_INT sign_int;
	sign_int = Bin2Dec(buffer, bin_arr_element - SIGN_BIT_LENGTH);

	// if the integer is negative
	if(sign == 1)
	{
		sign_int *= (-1);
	}

	return sign_int;
}


S_INT Get1Delta(U_INT byte_num_in, S_INT arr_in[], U_INT pos_in)
{
	return GetOasisSINT(byte_num_in, arr_in, pos_in);
}


U_INT Get2Delta(U_INT byte_num_in, S_INT arr_in[], U_INT pos_in)
{
	U_INT tmp;
	U_CHAR buffer[SCHAR_MAX];
	U_SHORT bin_arr_element = 0;

	for(U_INT i = 0; i < byte_num_in; i++)
	{
		Bin7ToArray(arr_in[pos_in + i], buffer, bin_arr_element);
	}

	InvertArray(buffer, bin_arr_element);

	tmp = Bin2Dec(buffer, bin_arr_element - DIR_BIT_2_DELTA_LENGTH);

	return tmp;
}


GDELTA2 GetGDelta2(S_INT oasis_arr[], U_INT byte_begin_pos, U_INT &byte_num_out)
{
	GDELTA2 gdelta2;
	U_CHAR buffer[USHRT_MAX];
	U_SHORT arr_element = 0;
	(VOID *) memset(&gdelta2, 0, sizeof(GDELTA2));
	byte_num_out = 0;

	U_INT byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);

	//calculate the first number
	for (U_INT i = 0; i < byte_num; i++)
	{
		Bin7ToArray(oasis_arr[byte_begin_pos + i], buffer, arr_element);
	}

	// inverse the binary array
	InvertArray(buffer, arr_element);

	// bit 0 of the first integer is 1
	// bit 1 of the first integer is the x-direction (0 for east, 1 for west)
	// the remaining bits is magnitude
	gdelta2.first_num = Bin2Dec(buffer, arr_element - 2);

	if (buffer[arr_element - 2]) // 1 for west --> negative
	{
		gdelta2.first_num *= (-1);
	}
	else // 0 for east --> positive
	{
		// do nothing
	}

	// update byte_begin_pos
	byte_begin_pos += byte_num;

	// update rep_length_out fist time
	byte_num_out += byte_num;

	// calculate the second number
	// bit 0 of the second integer is the y-direction (0 for north, 1 for south)
	// the remaining bits is magnitude
	byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
	gdelta2.second_num = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

	// update rep_length_out second time
	byte_num_out += byte_num;

	return gdelta2;
}


GDELTA1 GetGDelta1(S_INT oasis_arr[], U_INT byte_begin_pos, U_INT &byte_num_out)
{
	GDELTA1 gdelta1;
	(VOID *) memset(&gdelta1, 0, sizeof(GDELTA1));
	byte_num_out = 0;

	U_INT byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);


	gdelta1.value = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
	gdelta1.dir = ((gdelta1.value & 0x0e) >> 1);

	switch(gdelta1.dir)
	{
		case 0: // east
			gdelta1.x = gdelta1.value >> 4;
			gdelta1.y = 0;
			break;

		case 1: // north
			gdelta1.x = 0;
			gdelta1.y = gdelta1.value >> 4;
			break;

		case 2: // west
			gdelta1.x = (-1) * (gdelta1.value >> 4);
			gdelta1.y = 0;
			break;

		case 3: // south
			gdelta1.x = 0;
			gdelta1.y = (-1) * (gdelta1.value >> 4);
			break;

		case 4: // north-east
			gdelta1.x = gdelta1.value >> 4;
			gdelta1.y = gdelta1.value >> 4;
			break;

		case 5: // north-west
			gdelta1.x = (-1) * (gdelta1.value >> 4);
			gdelta1.y = gdelta1.value >> 4;
			break;

		case 6: // south-west
			gdelta1.x = (-1) * (gdelta1.value >> 4);
			gdelta1.y = (-1) * (gdelta1.value >> 4);
			break;

		case 7: // south-east
			gdelta1.x = gdelta1.value >> 4;
			gdelta1.y = (-1) * (gdelta1.value >> 4);
			break;
	}

	byte_num_out += byte_num;

	return gdelta1;
}


VOID ShowHelp()
{
	printf("NAME:\n\tDumping OASIS File Program\n\n");
	printf("SYNOPSIS:\n\t<program> [option] <input_file> <output_file>\n\n");
	printf("DESCRIPTION:\n\t-r: select the real unit");
	printf("\n\tinput_file: .oas");
	printf("\n\toutput_file: .txt\n");

	return;
}


VOID ShowError()
{
	printf("[ERROR] The parameters of program are not exact!\n");
	printf("See help of program: <program> -help\n");

	return;
}


BOOL Dialog(S_INT argc_in, S_CHAR *argv_arr_in[], BOOL &unit_select_out, S_CHAR *input_file_out, S_CHAR *output_file_out)
{
	BOOL flag = FALSE;

	S_INT i = 0;

	while(i < argc_in)
	{
		if(!strcmp(argv_arr_in[i], "-help"))
		{
				ShowHelp();

				return FALSE;
		}

		i++;
	}

	if((argc_in < 2) || (argc_in > 4))
	{
		ShowError();

		return FALSE;
	}
	else if(argc_in == 2)
	{
		S_INT i = 0;
		while(i < argc_in)
		{
			if(!strcmp(argv_arr_in[i], "-r"))
			{
					flag = TRUE;
			}

			i++;
		}

		if(flag)
		{
			printf("Program run with \"real\" unit\n");
			printf("The parameters are not enough\n");
			printf("See help of program: <program> -help\n");
			flag = FALSE;

			return FALSE;
		}
		else
		{
			printf("Program run with \"integer\" unit\n");
			printf("The parameters are not enough\n");
			printf("See help of program: <program> -help\n");
			flag = FALSE;

			return FALSE;
		}
	}
	else if (argc_in == 3)
	{
		S_INT i = 0;
		while(i < argc_in)
		{
			if(!strcmp(argv_arr_in[i], "-r"))
			{
					flag = TRUE;
			}

			i++;
		}

		if(flag)
		{
			printf("Program run with \"real\" unit\n");
			printf("The parameters are not enough\n");
			printf("See help of program: <program> -help\n");
			flag = FALSE;

			return FALSE;
		}
		else
		{
			printf("Program run with \"integer\" unit\n");
			strcpy(input_file_out, argv_arr_in[1]);
			strcpy(output_file_out, argv_arr_in[2]);

			// set the unit_select_out to FALSE
			unit_select_out = FALSE;
		}
	}
	else if(argc_in == 4)
	{
		S_INT i = 0;
		while(i < argc_in)
		{
			if(!strcmp(argv_arr_in[i], "-r"))
			{
				flag = TRUE;
			}

			i++;
		}

		if(flag)
		{
			printf("Program run with \"real\" unit\n");

			if(!strcmp(argv_arr_in[1], "-r"))
			{
				strcpy(input_file_out, argv_arr_in[2]);
				strcpy(output_file_out, argv_arr_in[3]);
			}
			else if(!strcmp(argv_arr_in[2], "-r"))
			{
				strcpy(input_file_out, argv_arr_in[1]);
				strcpy(output_file_out, argv_arr_in[3]);
			}
			else if(!strcmp(argv_arr_in[3], "-r"))
			{
				strcpy(input_file_out, argv_arr_in[1]);
				strcpy(output_file_out, argv_arr_in[2]);
			}

			// set the unit_select_out to TRUE
			unit_select_out = TRUE;

			flag = FALSE;
		}
		else
		{
			ShowError();

			return FALSE;
		}
	}

	return TRUE;
}


