/************************************/
/* File: handle_oasis.cxx 			*/
/* Revision: 1.0 (9-Oct-17)			*/
/* Author: Tran Bao Trung			*/
/************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oasisHandle.h"


S_INT main(S_INT argc, S_CHAR *argv[])
{
	S_INT *oasis_arr;

	// allocate array dynamic memory
	oasis_arr = new S_INT [ARRAY_MAX];

	U_INT oasis_length = 0;

//	BOOL unit_select = TRUE; // true: real, false: integer
	BOOL unit_select = FALSE; // true: real, false: integer

//	BOOL unit_select;
//	S_CHAR input_file[UCHAR_MAX];
//	S_CHAR output_file[UCHAR_MAX];
//
//	S_CHAR *arg_arr[SCHAR_MAX];
//
//	for(S_INT i = 0; i < SCHAR_MAX; i++)
//	{
//		arg_arr[i] = new S_CHAR[SCHAR_MAX];
//	}
//
//	for(S_INT i = 0; i < argc; i++)
//	{
//		strcpy(arg_arr[i], argv[i]);
//	}
//
//	if(!Dialog(argc, arg_arr, unit_select, input_file, output_file))
//	{
//		// free dynamic allocation
//		delete [] oasis_arr;
//
//		for(S_INT i = 0; i < SCHAR_MAX; i++)
//		{
//			delete [] arg_arr[i];
//		}
//
//		return 0;
//	}


	const S_CHAR input_file[] = "test.oas";
//	const S_CHAR input_file[] = "test40nm_hier0.oas";
//	const S_CHAR input_file[] = "test50nm.oas";
//	const S_CHAR input_file[] = "cell_6.oas";
//	const S_CHAR input_file[] = "polygon_1.oas";
	const S_CHAR output_file[] = "output.txt";

	GetOasis2Array(oasis_arr, oasis_length, input_file);

	if(!oasis_length)
	{
		// free dynamic allocation
		delete [] oasis_arr;

		for(S_INT i = 0; i < SCHAR_MAX; i++)
		{
//			delete [] arg_arr[i];
		}

		return 0;
	}

	printf("Length of OASIS file = %u bytes\n", oasis_length);

	// main process
	WriteOasis2Text(oasis_arr, oasis_length, input_file, output_file, unit_select);

	printf("Task done!\n");

	// free dynamic allocation
	delete [] oasis_arr;

	for(S_INT i = 0; i < SCHAR_MAX; i++)
	{
//		delete [] arg_arr[i];
	}


	return 0;
}


VOID WriteOasis2Text(S_INT oasis_arr[], U_INT oasis_length_in, const S_CHAR input_file_in[], const S_CHAR output_file_in[], BOOL unit_select_in)
{
	OASIS oasis;
	(VOID *) memset(&oasis, 0, sizeof(OASIS));

	CELL cell;
	(VOID *) memset(&cell, 0, sizeof(CELL));

	U_INT i = 0;

	FILE *read_file;
	FILE *write_file;
	read_file = fopen(input_file_in, "r");
	write_file = fopen(output_file_in, "w");
	S_INT ch = getc(read_file);

	strcpy(oasis.magic_bytes, "%SEMI-OASI<CR><CL>");
	fprintf(write_file, "000000000\tmagic_byte \"%s\"", oasis.magic_bytes);
	strcpy(oasis.version, "1.0");

	oasis.unit_select = unit_select_in;
	oasis.end_begin_pos = oasis_length_in - END_LENGTH;
	oasis.remain_last_pos = oasis_length_in - END_LENGTH - 1;

	while(i < oasis_length_in)
	{
		/********** dump START **********/
		if(i == START_ID_POS)
		{
			fprintf(write_file, "\n\n%09d\tSTART id=%u ", START_ID_POS, oasis_arr[START_ID_POS]);
		}

		// dump version
		if(i == UNIT_POS - 1)
		{
			fprintf(write_file, "version=\"%s\" ", oasis.version);
		}

		// dump unit
		oasis.unit_begin_pos = MAGIC_BYTE_LENGTH + START_ID_LENGTH + VERSION_LENGTH;
		oasis.unit = GetUnit(oasis_arr, oasis.unit_begin_pos, oasis.unit_length);

		if(i == oasis.unit_begin_pos)
		{
			switch(oasis_arr[oasis.unit_begin_pos])
			{
				case 7:
					fprintf(write_file, "unit=real7(%0.1lf)", oasis.unit);
					break;
			}

		}

		// dump offset flag and table offsets
		oasis.offset_flag_pos = oasis.unit_begin_pos + oasis.unit_length;

		// define START length before check table offset
		oasis.start_length = VERSION_LENGTH + oasis.unit_length + START_ID_LENGTH + OFFSET_FLAG_LENGTH;

		// if table-offsets at START
		if(i == oasis.offset_flag_pos)
		{
			oasis.offset_flag = (BOOL) oasis_arr[oasis.offset_flag_pos];

			if(!oasis.offset_flag) // when offset_flag = 0
			{
				oasis.table_offsets_begin_pos = oasis.offset_flag_pos + OFFSET_FLAG_LENGTH;

				// get table-offsets
				for(U_CHAR j = 0; j < TABLE_OFFSETS_ELEMENTS; j++)
				{
					oasis.table_offsets[j] = GetTableOffsets(oasis_arr, oasis.table_offsets_begin_pos, oasis.table_offsets_length);
				}

				if(i == oasis.table_offsets_begin_pos + oasis.table_offsets_length)
				{
					for(U_INT i = 0; i < TABLE_OFFSETS_ELEMENTS; i += 2)
					{
						if(i == 0)
						{
							fprintf(write_file, "table_offsets=[(%u,%u) ", oasis.table_offsets[i], oasis.table_offsets[i + 1]);
						}
						else if (i == TABLE_OFFSETS_ELEMENTS - 2)
						{
							fprintf(write_file, "(%u,%u)] ", oasis.table_offsets[i], oasis.table_offsets[i + 1]);
						}
						else
						{
							fprintf(write_file, "(%u,%u) ", oasis.table_offsets[i], oasis.table_offsets[i + 1]);
						}

					}
				}

				// update START length if table offset at START
				oasis.start_length = 1 + VERSION_LENGTH + oasis.unit_length + 1 + oasis.table_offsets_length;
			}
		}

		/*********************************/



		/********** dump remaining record **********/
		if(i == MAGIC_BYTE_LENGTH + oasis.start_length)
		{
			oasis.next_rec_begin_pos = MAGIC_BYTE_LENGTH + oasis.start_length;
		}

		if((i >= oasis.next_rec_begin_pos) && (i < oasis.remain_last_pos))
		{
			// when encounter a new CELL --> reset the CELL structure
			if((oasis_arr[oasis.next_rec_begin_pos] == CELL13_ID) || (oasis_arr[oasis.next_rec_begin_pos] == CELL14_ID))
			{
				(VOID *) memset(&cell, 0, sizeof(CELL));
			}

			// call dump remaining records
			DumpRemainRecord(oasis.next_rec_begin_pos, oasis_arr, cell, oasis.unit, oasis.unit_select, oasis.next_rec_length, write_file);

			// update begin position of new record
			oasis.next_rec_begin_pos += oasis.next_rec_length;
		}



		/*********************************/


		/********** dump END record **********/
		if(i == oasis.end_begin_pos)
		{
			fprintf(write_file, "\n\n%09d\tEND id=%d ", oasis.end_begin_pos, ch);
		}

		//table-offsets at END
		if(oasis.offset_flag)
		{
			oasis.table_offsets_begin_pos = oasis.end_begin_pos + END_ID_LENGTH;
			oasis.table_offsets_last_pos = oasis.table_offsets_begin_pos;
			oasis.table_offsets_length = 0;

			for(U_CHAR j = 0; j < TABLE_OFFSETS_ELEMENTS; j++)
			{
				oasis.table_offsets[j] = GetTableOffsets(oasis_arr, oasis.table_offsets_last_pos, oasis.table_offsets_length);
			}

			if(i == oasis.end_begin_pos + oasis.table_offsets_length)
			{
				for(U_INT i = 0; i < TABLE_OFFSETS_ELEMENTS; i += 2)
				{
					if(i == 0)
					{
						fprintf(write_file, "table_offsets=[(%u,%u) ", oasis.table_offsets[i], oasis.table_offsets[i + 1]);
					}
					else if (i == TABLE_OFFSETS_ELEMENTS - 2)
					{
						fprintf(write_file, "(%u,%u)] ", oasis.table_offsets[i], oasis.table_offsets[i + 1]);
					}
					else
					{
						fprintf(write_file, "(%u,%u) ", oasis.table_offsets[i], oasis.table_offsets[i + 1]);
					}

				}
			}
		}

		// padding-string
		oasis.padding_string_length = DumpPaddingString(oasis.offset_flag, oasis.table_offsets_length);

		if(oasis.offset_flag) // table-offsets in END
		{
			if(i == oasis.table_offsets_begin_pos + oasis.table_offsets_length)
			{
				fprintf(write_file, "padding_string=%u ", oasis.padding_string_length);
			}
		}
		else // table-offsets in START
		{
			if(i == oasis.end_begin_pos + END_ID_LENGTH)
			{
				fprintf(write_file, "padding_string=%u ", oasis.padding_string_length);
			}
		}

		// validation-scheme
		oasis.scheme_begin_pos = oasis_length_in - VALIDATION_SIGN_LENGTH - 1;

		if(i == oasis.scheme_begin_pos)
		{
			CheckValidationScheme(write_file, (U_CHAR) ch);
			oasis.scheme_flag = ch;
		}

		if(oasis.scheme_flag > 0 && i > oasis.scheme_begin_pos && i < oasis_length_in)
		{
			if(i == oasis.scheme_begin_pos + 1)
			{
				fprintf(write_file, "validation_signature=%02x", ch);
			}
			else
			{
				fprintf(write_file, "%02x", ch);
			}
		}


		//continue to read the character from file
		ch = getc(read_file);

		//increase the counter value
		i++;
	}

	//close the file
	fclose(read_file);
	fclose(write_file);

	return;
}


VOID DumpRemainRecord(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, DOUBLE unit_in, BOOL unit_select_in, U_INT &rec_length, FILE *write_file)
{
	switch(oasis_arr[rec_begin_pos_in])
	{
		case CELLNAME3_ID:
		case CELLNAME4_ID:
			DumpCellName(rec_begin_pos_in, oasis_arr, rec_length, write_file);
			break;

		case TEXTSTRING5_ID:
			DumpTextString(rec_begin_pos_in, oasis_arr, rec_length, write_file);
			break;

		case PROPNAME7_ID:
		case PROPNAME8_ID:
			DumpPropName(rec_begin_pos_in, oasis_arr, rec_length, write_file);
			break;

		case PROPSTRING9_ID:
		case PROPSTRING10_ID:
			DumpPropString(rec_begin_pos_in, oasis_arr, rec_length, write_file);
			break;

		case CELL13_ID:
			DumpCell(rec_begin_pos_in, oasis_arr, rec_length, write_file);
			break;

		case XYABSOLUTE_ID:
			fputs("-------------do not implement XYABSOLUTE yet-------------\n", write_file);
			break;

		case XYRELATIVE_ID:
			DumpXYRelative(rec_begin_pos_in, oasis_arr, cell_out, rec_length, write_file);
			break;

		case PLACEMENT17_ID:
			DumpPlacement(rec_begin_pos_in, oasis_arr, cell_out, unit_in, unit_select_in, rec_length, write_file);
			break;

		case TEXT_ID:
			DumpText(rec_begin_pos_in, oasis_arr, cell_out, unit_in, unit_select_in, rec_length, write_file);
			break;

		case RECTANGLE_ID:
			DumpRectangle(rec_begin_pos_in, oasis_arr, cell_out, unit_in, unit_select_in, rec_length, write_file);
			break;

		case POLYGON_ID:
			DumpPolygon(rec_begin_pos_in, oasis_arr, cell_out, unit_in, unit_select_in, rec_length, write_file);
			break;

		case PATH_ID:
			fputs("\n\n-------------do not implement PATH yet-------------\n", write_file);
			rec_length = 13;
			break;

		case PROPERTY28_ID:
			DumpProperty(rec_begin_pos_in, oasis_arr, rec_length, write_file);
			break;

		default:
			break;

	}

	return;
}





