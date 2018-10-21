/************************************/
/* File: oasisHandle.cxx 			*/
/* Revision: 1.0 (5-Nov-17)			*/
/* Author: Tran Bao Trung			*/
/************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oasisHandle.h"


S_INT main(S_INT argc, S_CHAR *argv[])
{
	OASIS oasis;
	(VOID *) memset(&oasis, 0, sizeof(OASIS));

	// allocate array dynamic memory
	oasis.byte_arr = new S_INT [ARRAY_MAX];

	const S_CHAR input_file[] = "test.oas";
//	const S_CHAR input_file[] = "test50nm.oas";

	GetOasis2Array(oasis.byte_arr, oasis.length, input_file);

	DumpOasis(oasis);

	delete [] oasis.byte_arr;

	return 0;
}


VOID DumpOasis(OASIS oasis)
{
	strcpy(oasis.magic_bytes, "%SEMI-OASI<CR><CL>");
	printf("000000000\tmagic_byte \"%s\"", oasis.magic_bytes);

	oasis.rec_begin_pos = 13;

	while(oasis.rec_begin_pos < oasis.length - 1)
	{
		switch(oasis.byte_arr[oasis.rec_begin_pos])
		{
			case START_ID:
				DumpStart(oasis.start, oasis.byte_arr, oasis.rec_length);

				// update next record begin position
				oasis.rec_begin_pos += oasis.rec_length;
				break;

			case PROPERTY28_ID:
				return;
				break;
		}
	}


	return;
}

VOID DumpStart(START start, S_INT oasis_arr[], U_INT &rec_length)
{
	strcpy(start.version, "1.0");

	start.unit_begin_pos = MAGIC_BYTE_LENGTH + START_ID_LENGTH + VERSION_LENGTH;
	start.unit = GetUnit(oasis_arr, start.unit_begin_pos, start.unit_length);

	start.offset_flag_begin_pos = start.unit_begin_pos + start.unit_length;

//	printf("start.offset_flag_begin_pos = %u\n", start.offset_flag_begin_pos);

	start.offset_flag = (BOOL) oasis_arr[start.offset_flag_begin_pos];

	// table-offset at START
	if(!(start.offset_flag))
	{

	}

	printf("\n\n%09d\tSTART id=%u version=\"%s\"", START_ID_POS, oasis_arr[START_ID_POS], \
			start.version);

	switch(oasis_arr[start.unit_begin_pos])
	{
		case 7:
			printf(" unit=real7(%0.1lf)", start.unit);
			break;
	}

	// calculate record length
	rec_length = START_ID_LENGTH + VERSION_LENGTH + start.unit_length + OFFSET_FLAG_LENGTH;

	return;
}


