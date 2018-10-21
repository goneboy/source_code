#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "oasisHandle.h"


VOID DumpProperty(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	U_INT byte_begin_pos = rec_begin_pos_in;
	U_INT byte_num = 0;
	PROPERTY property;
	memset(&property, 0, sizeof(property));

	property.id = oasis_arr[byte_begin_pos];
	property.info_byte = oasis_arr[byte_begin_pos + PROPERTY_ID_LENGTH];

	U_CHAR UUUU = property.info_byte >> 4;
	BOOL U1 = property.info_byte & 0X80;
	BOOL U2 = property.info_byte & 0X40;
	BOOL U3 = property.info_byte & 0X20;
	BOOL U4 = property.info_byte & 0X10;
	BOOL V = property.info_byte & 0X08;
	BOOL C = property.info_byte & 0X04;
	BOOL N = property.info_byte & 0X02;
	BOOL S = property.info_byte & 0X01;

	fprintf(write_file, "\n\n%09d\tPROPERTY id=%u ", byte_begin_pos, property.id);
	fprintf(write_file, "UUUUVCNS=%d%d%d%d%d%d%d%d ", U1, U2, U3, U4, V, C, N, S);

	// C=1, the property name reference is explicit
	if(C)
	{
		// N=1, reference-number is present, refers to a PROPNAME record where the property name is stored
		if(N)
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos + PROPERTY_ID_LENGTH + INFO_BYTE_LENGTH);
			property.ref_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos + PROPERTY_ID_LENGTH + INFO_BYTE_LENGTH);
			fprintf(write_file, "ref_num=%u ", property.ref_num);

			// update byte_begin_pos
//			byte_begin_pos += (byte_num + PROPERTY_ID_LENGTH + INFO_BYTE_LENGTH);
		}
		// N=0, propname-string (n-string) is present and stores the property name locally
		else
		{
			// in the future
		}
	}
	// C=0, N is ignored, and the value of modal variable last-property-name is used instead
	else
	{

	}

	// update byte_begin_pos
	byte_begin_pos += (byte_num + PROPERTY_ID_LENGTH + INFO_BYTE_LENGTH);

	// V = 1
	// UUUU must be 0, and modal variable last-value-list supplies the value list
	if(V)
	{
		fputs("(same ref_num)", write_file);
//		byte_begin_pos += 2;
	}
	else // V = 0
	{
		if(UUUU == 15)
		{
			property.prop_value_count = oasis_arr[byte_begin_pos];

		}
		else
		{
			property.prop_value_count = UUUU;
			fprintf(write_file, "value_count=%u ", property.prop_value_count);

			for(S_CHAR i = 0; i < property.prop_value_count; i++)
			{
				property.value_type = oasis_arr[byte_begin_pos];

				DumpPropValue(oasis_arr, property.value_type, byte_begin_pos, write_file);
			}
		}
	}

	//update the record length
	rec_length_out = byte_begin_pos - rec_begin_pos_in;

	return;
}


VOID DumpPropValue(S_INT oasis_arr[], S_CHAR value_type_in, U_INT &value_begin_pos_out, FILE *write_file)
{
	U_INT byte_num;
	U_INT value_ref_num;
	U_INT prop_value;

	switch(value_type_in)
	{
		case PROP_U_INT: // type 8: unsigned integer
			byte_num = GetIntegerByteNum(oasis_arr, value_begin_pos_out + 1);
			prop_value = GetOasisUINT(byte_num, oasis_arr, value_begin_pos_out + 1);
			value_begin_pos_out += (byte_num + 1);
			fprintf(write_file, "type=%u value=%u", value_type_in, prop_value);
			break;

		case PROP_REF_NUM_A:
		case PROP_REF_NUM_B:
		case PROP_REF_NUM_N:
			byte_num = GetIntegerByteNum(oasis_arr, value_begin_pos_out + 1);
			value_ref_num = GetOasisUINT(byte_num, oasis_arr, value_begin_pos_out + 1);
			value_begin_pos_out += (byte_num + 1);
			fprintf(write_file, "type=%u value=%u", value_type_in, value_ref_num);
//			fprintf(write_file, "type=%u value=propstring-ref", value_type_in);
//			fputs("propstring-ref", write_file);
			break;
	}

	return;
}


VOID DumpCell(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	CELL cell;
	(VOID *) memset(&cell, 0, sizeof(cell));
	U_INT byte_begin_pos = rec_begin_pos_in;
	U_INT byte_num;

	cell.id = oasis_arr[byte_begin_pos];
	byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos + CELL_ID_LENGTH);

	cell.ref_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos + CELL_ID_LENGTH);

	// update byte_begin_pos
	byte_begin_pos += (byte_num + CELL_ID_LENGTH);

	fprintf(write_file, "\n\n%09d\tCELL id=%u ref_num=%u", rec_begin_pos_in, cell.id, cell.ref_num);

	// calculate
	rec_length_out = byte_begin_pos - rec_begin_pos_in;

	return;
}


VOID DumpXYRelative(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, U_INT &rec_length_out, FILE *write_file)
{
	cell_out.xyrelative.id = oasis_arr[rec_begin_pos_in];

	fprintf(write_file, "\n\n%09d\tXYRELATIVE id=%u", rec_begin_pos_in, cell_out.xyrelative.id);

	cell_out.relative_flag = TRUE;

	rec_length_out = XYRELATIVE_ID_LENGTH;

	return;
}


VOID DumpRectangle(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, DOUBLE unit_in, BOOL unit_select_in, U_INT &rec_length_out, FILE *write_file)
{
	U_INT byte_num;
	U_INT byte_begin_pos = rec_begin_pos_in + 2;

	cell_out.rectangle.id = oasis_arr[rec_begin_pos_in];
	cell_out.rectangle.info_byte = oasis_arr[rec_begin_pos_in + RECTANGLE_ID_LENGTH];

	BOOL S = cell_out.rectangle.info_byte & 0x80;
	BOOL W = cell_out.rectangle.info_byte & 0x40;
	BOOL H = cell_out.rectangle.info_byte & 0x20;
	BOOL X = cell_out.rectangle.info_byte & 0x10;
	BOOL Y = cell_out.rectangle.info_byte & 0x08;
	BOOL R = cell_out.rectangle.info_byte & 0x04;
	BOOL D = cell_out.rectangle.info_byte & 0x02;
	BOOL L = cell_out.rectangle.info_byte & 0x01;

	fprintf(write_file, "\n\n%09d\tRECTANGLE id=%u ", rec_begin_pos_in, cell_out.rectangle.id);
	fprintf(write_file, "SWHXYRDL=%d%d%d%d%d%d%d%d ", S, W, H, X, Y, R, D, L);

	if(L) // layer-number is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.rectangle.layer_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "layer=%u ", cell_out.rectangle.layer_num);
		byte_begin_pos += byte_num;
	}
	// when layer-number is unspecified, it assumes the value of modal variable layer respectively
	else
	{

	}

	if(D) // datatype-number is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.rectangle.datatype_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "datatype=%u ", cell_out.rectangle.datatype_num);

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}
	// when datatype-number is unspecified, it assumes the value of modal variable layer respectively
	else
	{

	}

	if(S) // rectangle is square, H must be 0
	{
		if(W) // width is present, is used for both dimensions of the rectangle
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			cell_out.rectangle.width = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);

			if(unit_select_in)
			{
				fprintf(write_file, "(square) width=%.3lf ", (DOUBLE) cell_out.rectangle.width / unit_in);
			}
			else
			{
				fprintf(write_file, "(square) width=%u ", cell_out.rectangle.width);
			}

			// update byte_begin_pos
			byte_begin_pos += byte_num;
		}
		else // width is not present
		{
			fputs("(square) (same previous width) ", write_file);
			cell_out.rectangle.width = cell_out.rectangle.height = cell_out.rectangle.geometry_w;
		}
	}
	else // rectangle is non-square
	{
		if(W) // width is present
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			cell_out.rectangle.width = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);

			if(unit_select_in)
			{
				fprintf(write_file, " width=%.3lf ", (DOUBLE) cell_out.rectangle.width / unit_in);
			}
			else
			{
				fprintf(write_file, " width=%u ", cell_out.rectangle.width);
			}

			// update byte_begin_pos
			byte_begin_pos += byte_num;
		}
		// when width is unspecified, the value of modal variable geometry-w is used instead
		else
		{
			fputs("(same previous width) ", write_file);
			cell_out.rectangle.width = cell_out.rectangle.geometry_w;
		}

		if(H) // height is present
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			cell_out.rectangle.height = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);

			if(unit_select_in)
			{
				fprintf(write_file, " height=%.3lf ", (DOUBLE) cell_out.rectangle.height / unit_in);
			}
			else
			{
				fprintf(write_file, " height=%u ", cell_out.rectangle.height);
			}

			// update byte_begin_pos
			byte_begin_pos += byte_num;
		}
		// when height is unspecified, the value of modal variable geometry-h is used instead
		else
		{
			fputs("(same previous height) ", write_file);
			cell_out.rectangle.height = cell_out.rectangle.geometry_h;
		}
	}


	if(X) // x is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.rectangle.x = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

		if(unit_select_in)
		{
			fprintf(write_file, " x=%.3lf ", (DOUBLE) cell_out.rectangle.x / unit_in);
		}
		else
		{
			fprintf(write_file, " x=%d ", cell_out.rectangle.x);
		}

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}
	// x is unspecified, the value of modal variable geometry-x, respectively, is used instead
	else
	{
		cell_out.rectangle.x = 0;
	}

	if(Y) // y is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.rectangle.y = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

		if(unit_select_in)
		{
			fprintf(write_file, " y=%.3lf ", (DOUBLE) cell_out.rectangle.y / unit_in);
		}
		else
		{
			fprintf(write_file, " y=%d ", cell_out.rectangle.y);
		}

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}
	// y is unspecified, the value of modal variable geometry-y, respectively, is used instead
	else
	{
		cell_out.rectangle.y = 0;
	}

	if(R) // repetition is present
	{
		DumpRepetition(byte_begin_pos, oasis_arr, unit_in, unit_select_in, byte_num, write_file);

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}

	// in relative mode
	if(cell_out.relative_flag)
	{
		// calculate actual (x, y) coordinate
		// the actual x-coordinate is computed as the sum of the x value and its corresponding modal position variable
		// the actual y-coordinate is computed as the sum of the y value and its corresponding modal position variable
		cell_out.rectangle.actual_x = cell_out.rectangle.x + cell_out.rectangle.geometry_x;
		cell_out.rectangle.actual_y = cell_out.rectangle.y + cell_out.rectangle.geometry_y;
	}
	// in absolute mode
	else
	{
		// calculate actual (x, y) coordinate
		cell_out.rectangle.actual_x = cell_out.rectangle.x;
		cell_out.rectangle.actual_y = cell_out.rectangle.y;
	}

	if(unit_select_in)
	{
		fprintf(write_file, " actual_xy=(%.3lf,%.3lf)", (DOUBLE) cell_out.rectangle.actual_x / unit_in, \
							(DOUBLE) cell_out.rectangle.actual_y / unit_in);
	}
	else
	{
		fprintf(write_file, " actual_xy=(%d,%d)", cell_out.rectangle.actual_x, cell_out.rectangle.actual_y);
	}

	// update geometry-x and geometry-y
	// the corresponding modal position variables are always updated with the actual (x,y) coordinate position
	cell_out.rectangle.geometry_x = cell_out.rectangle.actual_x;
	cell_out.rectangle.geometry_y = cell_out.rectangle.actual_y;

	// update geometry-w and geometry-h
	cell_out.rectangle.geometry_w = cell_out.rectangle.width;
	cell_out.rectangle.geometry_h = cell_out.rectangle.height;


	// calculate the record length
	rec_length_out = byte_begin_pos - rec_begin_pos_in;

	return;
}


VOID DumpCellName(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	CELLNAME cellname;
	(VOID *) memset(&cellname, 0, sizeof(cellname));
	U_INT byte_num;

	cellname.id = (U_CHAR) oasis_arr[rec_begin_pos_in];
	byte_num = GetIntegerByteNum(oasis_arr, rec_begin_pos_in + 1);
	cellname.test_byte_num = GetOasisUINT(byte_num, oasis_arr, rec_begin_pos_in + 1);

	fprintf(write_file, "\n\n%09d\tCELLNAME id=%u cellname=\"", rec_begin_pos_in, cellname.id);

	for(U_INT i = 0; i < cellname.test_byte_num; i++)
	{
		fprintf(write_file, "%c", oasis_arr[rec_begin_pos_in + 2 + i]);
	}
	fputs("\"", write_file);

	rec_length_out = 2 + cellname.test_byte_num;

	return;
}

VOID DumpPropName(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	PROPNAME propname;
	(VOID *) memset(&propname, 0, sizeof(propname));
	U_INT byte_num;

	propname.id = (U_CHAR) oasis_arr[rec_begin_pos_in];
	byte_num = GetIntegerByteNum(oasis_arr, rec_begin_pos_in + 1);
	propname.test_byte_num = GetOasisUINT(byte_num, oasis_arr, rec_begin_pos_in + 1);

	fprintf(write_file, "\n\n%09d\tPROPNAME id=%u propname=\"", rec_begin_pos_in, propname.id);

	for(U_INT i = 0; i < propname.test_byte_num; i++)
	{
		fprintf(write_file, "%c", oasis_arr[rec_begin_pos_in + 2 + i]);
	}
	fputs("\"", write_file);

	rec_length_out = 2 + propname.test_byte_num;

	return;
}


VOID DumpPropString(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	PROPSTRING propstring;
	(VOID *) memset(&propstring, 0, sizeof(propstring));
	U_INT byte_num;

	propstring.id = (U_CHAR) oasis_arr[rec_begin_pos_in];
	byte_num = GetIntegerByteNum(oasis_arr, rec_begin_pos_in + 1);
	propstring.test_byte_num = GetOasisUINT(byte_num, oasis_arr, rec_begin_pos_in + 1);

	fprintf(write_file, "\n\n%09d\tPROPSTRING id=%u propstring=\"", rec_begin_pos_in, propstring.id);

	for(U_INT i = 0; i < propstring.test_byte_num; i++)
	{
		fprintf(write_file, "%c", oasis_arr[rec_begin_pos_in + 2 + i]);
	}
	fputs("\"", write_file);

	rec_length_out = 2 + propstring.test_byte_num;

	return;
}


VOID CheckValidationScheme(FILE *write_file, U_CHAR scheme_in)
{
	if(scheme_in == 0)
	{
		fputs("validation_scheme=No validation ", write_file);
	}
	else if(scheme_in == 1)
	{
		fputs("validation_scheme=CRC32 ", write_file);
	}
	else /* scheme_in == 2 */
	{
		fputs("validation_scheme=CHECKSUM32 ", write_file);
	}

	return;
}


U_INT DumpPaddingString(BOOL offset_flag_in, U_INT table_offsets_length_in)
{
	if(offset_flag_in)
	{
		return END_LENGTH - table_offsets_length_in - VALIDATION_SIGN_LENGTH - 1 - 1;
	}
	else
	{
		return END_LENGTH - VALIDATION_SIGN_LENGTH - 1 - 1;
	}
}


VOID DumpPolygon(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, DOUBLE unit_in, BOOL unit_select_in, U_INT &rec_length_out, FILE *write_file)
{
	POINTLIST pointlist_arr[UCHAR_MAX];

	for(U_CHAR i = 0; i < UCHAR_MAX; i++)
	{
		(VOID *) memset(&pointlist_arr[i], 0, sizeof(pointlist_arr[i]));
	}

	U_INT byte_num;
	U_INT byte_begin_pos = rec_begin_pos_in + 2;

	cell_out.polygon.info_byte = oasis_arr[rec_begin_pos_in + POLYGON_ID_LENGTH];
	cell_out.polygon.id = oasis_arr[rec_begin_pos_in];

	BOOL P = cell_out.polygon.info_byte & 0x20;
	BOOL X = cell_out.polygon.info_byte & 0x10;
	BOOL Y = cell_out.polygon.info_byte & 0x08;
	BOOL R = cell_out.polygon.info_byte & 0x04;
	BOOL D = cell_out.polygon.info_byte & 0x02;
	BOOL L = cell_out.polygon.info_byte & 0x01;

	fprintf(write_file, "\n\n%09d\tPOLYGON id=%u ", rec_begin_pos_in, cell_out.polygon.id);
	fprintf(write_file, "00PXYRDL=00%d%d%d%d%d%d ", P, X, Y, R, D, L);

	if(L) // layer-number is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.polygon.layer_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "layer=%u ", cell_out.polygon.layer_num);
		byte_begin_pos += byte_num;
	}
	// when layer-number is unspecified, they assume the value of modal variable layer respectively
	else
	{
		// For future
	}

	if(D) // datatype-number is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.polygon.datatype_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "datatype=%u ", cell_out.polygon.datatype_num);
		byte_begin_pos += byte_num;
	}
	// when datatype-number is unspecified, they assume the value of modal variable datatype, respectively
	else
	{
		// For future
	}

	if(P) // point list is present
	{
		DumpPointList(byte_begin_pos, oasis_arr, cell_out.polygon, pointlist_arr, cell_out.polygon.pointlist.pointlist_length);
		fprintf(write_file, "pointlist(type=%u vertex_count=%u delta=[", cell_out.polygon.pointlist.pointlist_type, cell_out.polygon.pointlist.vertex_count);

		for(U_INT i = 0; i < cell_out.polygon.pointlist.vertex_count; i++)
		{
			if(i == cell_out.polygon.pointlist.vertex_count - 1)
			{
//				fprintf(write_file, "(%.3lf,%.3lf)] ", (DOUBLE) pointlist_arr[i].x / unit_in, (DOUBLE) pointlist_arr[i].y / unit_in);

				if(unit_select_in)
				{
					fprintf(write_file, "%.3lf]) ", (DOUBLE) cell_out.polygon.pointlist.delta_arr[i] / unit_in);
				}
				else
				{
					fprintf(write_file, "%d]) ", cell_out.polygon.pointlist.delta_arr[i]);
				}
			}
			else
			{
//				fprintf(write_file, "(%.3lf,%.3lf) ", (DOUBLE) pointlist_arr[i].x / unit_in, (DOUBLE) pointlist_arr[i].y / unit_in);

				if(unit_select_in)
				{
					fprintf(write_file, "%.3lf ", (DOUBLE) cell_out.polygon.pointlist.delta_arr[i] / unit_in);
				}
				else
				{
					fprintf(write_file, "%d ", cell_out.polygon.pointlist.delta_arr[i]);
				}
			}
		}

		// update byte_begin_pos
		byte_begin_pos += cell_out.polygon.pointlist.pointlist_length;
	}

	if(X) // x is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.polygon.x = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

		if(unit_select_in)
		{
			fprintf(write_file, "x=%.3lf ", (DOUBLE) cell_out.polygon.x / unit_in);
		}
		else
		{
			fprintf(write_file, "x=%d ", cell_out.polygon.x);
		}

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}
	// x is unspecified, the value of modal variable geometry-x, respectively, is used instead
	else
	{
		cell_out.polygon.x = 0;
	}

	if(Y) // y is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.polygon.y = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

		if(unit_select_in)
		{
			fprintf(write_file, "y=%.3lf ", (DOUBLE) cell_out.polygon.y / unit_in);
		}
		else
		{
			fprintf(write_file, "y=%d ", cell_out.polygon.y);
		}

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}
	// y is unspecified, the value of modal variable geometry-y, respectively, is used instead
	else
	{
		cell_out.polygon.y = 0;
	}

	if(R) // repetition is present
	{
		// in the future
	}

	// in relative mode
	// in this mode, the actual x-coordinate is the sum of the x value and its corresponding modal position variable
	// the actual y-coordinate is the sum of the y value and its corresponding modal position variable
	if(cell_out.relative_flag)
	{
		// calculate actual (x, y) coordinate
		cell_out.polygon.actual_x = cell_out.polygon.x + cell_out.polygon.geometry_x;
		cell_out.polygon.actual_y = cell_out.polygon.y + cell_out.polygon.geometry_y;
	}
	// in absolute mode
	// in this mode, explicit x and y values, when present, are used directly as the actual (x,y) coordinates
	else
	{
		// calculate actual (x, y) coordinate
		cell_out.polygon.actual_x = cell_out.polygon.x;
		cell_out.polygon.actual_y = cell_out.polygon.y;
	}

	if(unit_select_in)
	{
		fprintf(write_file, " initial_vertex=(%.3lf,%.3lf)", (DOUBLE) cell_out.polygon.actual_x / unit_in, \
							(DOUBLE) cell_out.polygon.actual_y / unit_in);
	}
	else
	{
		fprintf(write_file, " initial_vertex=(%d,%d)", cell_out.polygon.actual_x, cell_out.polygon.actual_y);
	}


	// show actual vertex of polygon
	fputs(" actual_vertex=[", write_file);

	if(unit_select_in)
	{
		for(U_INT i = 0; i < cell_out.polygon.pointlist.vertex_count; i++)
		{
			if(i == cell_out.polygon.pointlist.vertex_count - 1)
			{
				fprintf(write_file, "(%.3lf,%.3lf)] ", (DOUBLE) (cell_out.polygon.actual_x + pointlist_arr[i].x) / unit_in, \
						(DOUBLE) (cell_out.polygon.actual_y + pointlist_arr[i].y) / unit_in);
			}
			else
			{
				fprintf(write_file, "(%.3lf,%.3lf) ", (DOUBLE) (cell_out.polygon.actual_x + pointlist_arr[i].x) / unit_in,\
						(DOUBLE) (cell_out.polygon.actual_y + pointlist_arr[i].y) / unit_in);
			}

		}
	}
	else
	{
		for(U_INT i = 0; i < cell_out.polygon.pointlist.vertex_count; i++)
		{
			if(i == cell_out.polygon.pointlist.vertex_count - 1)
			{
				fprintf(write_file, "(%d,%d)] ", (cell_out.polygon.actual_x + pointlist_arr[i].x), (cell_out.polygon.actual_y + pointlist_arr[i].y));
			}
			else
			{
				fprintf(write_file, "(%d,%d) ", (cell_out.polygon.actual_x + pointlist_arr[i].x), (cell_out.polygon.actual_y + pointlist_arr[i].y));
			}

		}
	}

	// update geometry-x and geometry-y
	// the corresponding modal position variables are always updated with the actual (x,y) coordinate position
	cell_out.polygon.geometry_x = cell_out.polygon.actual_x;
	cell_out.polygon.geometry_y = cell_out.polygon.actual_y;

	// calculate the record length
	rec_length_out = byte_begin_pos - rec_begin_pos_in;

	return;
}


VOID DumpPointList(U_INT pointlist_begin_pos_in, S_INT oasis_arr[], POLYGON &polygon_out, POINTLIST pointlist_arr[], U_INT &pointlist_length)
{
	S_INT pointlist_type = oasis_arr[pointlist_begin_pos_in];

	switch(pointlist_type)
	{
		case POINTLIST_0:
			DumpPointList0(pointlist_begin_pos_in, oasis_arr, polygon_out, pointlist_arr, pointlist_length);
			break;

		case POINTLIST_1:
			DumpPointList1(pointlist_begin_pos_in, oasis_arr, polygon_out, pointlist_arr, pointlist_length);
			break;

		default:
			break;

	}

	return;
}


VOID DumpPointList0(U_INT pointlist_begin_pos_in, S_INT oasis_arr[], POLYGON &polygon_out, POINTLIST pointlist_arr[], U_INT &pointlist_length)
{
	U_INT byte_num;
	U_INT byte_pos;
	pointlist_length = 0;
	polygon_out.pointlist.x = 0;
	polygon_out.pointlist.y = 0;

	byte_pos = pointlist_begin_pos_in + 1;
	byte_num = GetIntegerByteNum(oasis_arr, byte_pos);

	//calculate the vertex count
	polygon_out.pointlist.vertex_count = GetOasisUINT(byte_num, oasis_arr, byte_pos);

	//update the byte position
	byte_pos += byte_num;

	for(U_INT i = 0; i < polygon_out.pointlist.vertex_count; i++)
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_pos);

		if(i % 2 == 0)
		{
			polygon_out.pointlist.x += GetOasisSINT(byte_num, oasis_arr, byte_pos);
		}
		else
		{
			polygon_out.pointlist.y += GetOasisSINT(byte_num, oasis_arr, byte_pos);
		}


		// calculate the delta array
		polygon_out.pointlist.delta_arr[i] = GetOasisSINT(byte_num, oasis_arr, byte_pos);

		pointlist_arr[i].x = polygon_out.pointlist.x;
		pointlist_arr[i].y = polygon_out.pointlist.y;

		// update the byte position
		byte_pos += byte_num;
	}

	// set pointlist type to 0
	polygon_out.pointlist.pointlist_type = POINTLIST_0;

	//update the pointlist length
	pointlist_length = byte_pos - pointlist_begin_pos_in;

	return;
}


VOID DumpPointList1(U_INT pointlist_begin_pos_in, S_INT oasis_arr[], POLYGON &polygon_out, POINTLIST pointlist_arr[], U_INT &pointlist_length)
{
	U_INT byte_num;
	U_INT byte_pos;
	pointlist_length = 0;
	polygon_out.pointlist.x = 0;
	polygon_out.pointlist.y = 0;

	byte_pos = pointlist_begin_pos_in + 1;
	byte_num = GetIntegerByteNum(oasis_arr, byte_pos);

	// calculate the vertex count
	polygon_out.pointlist.vertex_count = GetOasisUINT(byte_num, oasis_arr, byte_pos);

	// update the byte position
	byte_pos += byte_num;

	for(U_INT i = 0; i < polygon_out.pointlist.vertex_count; i++)
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_pos);

		if(i % 2 == 0)
		{
			polygon_out.pointlist.y += GetOasisSINT(byte_num, oasis_arr, byte_pos);
		}
		else
		{
			polygon_out.pointlist.x += GetOasisSINT(byte_num, oasis_arr, byte_pos);
		}

		// calculate the delta array
		polygon_out.pointlist.delta_arr[i] = GetOasisSINT(byte_num, oasis_arr, byte_pos);

		pointlist_arr[i].x = polygon_out.pointlist.x;
		pointlist_arr[i].y = polygon_out.pointlist.y;

		// update the byte position
		byte_pos += byte_num;
	}

	// set pointlist type to 1
	polygon_out.pointlist.pointlist_type = 1;

	//update the pointlist length
	pointlist_length = byte_pos - pointlist_begin_pos_in;

	return;
}



U_INT GetTableOffsets(S_INT oasis_arr[], U_INT &table_offsets_pos_out, U_INT &table_offsets_length_out)
{
	U_INT value;
	U_INT byte_num = GetIntegerByteNum(oasis_arr, table_offsets_pos_out);

	value = GetOasisUINT(byte_num, oasis_arr, table_offsets_pos_out);

	//update table offsets position
	table_offsets_pos_out += byte_num;

	//update table offsets length
	table_offsets_length_out += byte_num;

	return value;
}


VOID DumpRepetition(U_INT rep_begin_pos_in, S_INT oasis_arr[], DOUBLE unit_in, BOOL unit_select_in, U_INT &rep_length_out, FILE *write_file)
{
	U_INT byte_begin_pos = rep_begin_pos_in + REPETITION_TYPE_LENGTH;
	U_INT rep_type = oasis_arr[rep_begin_pos_in];
	U_INT dimension;
	U_INT byte_num;
	GDELTA2 gdelta2;
	GDELTA1 gdelta1;
	(VOID *) memset(&gdelta1, 0, sizeof(GDELTA1));

	S_INT x = 0;
	S_INT y = 0;
	U_INT P;
	U_INT grid;

	switch(rep_type)
	{
		case REPETITION_0: /* re-use the previous repetition definition */
			fprintf(write_file, "repetition((type=%u), reuse the previous repetition definition)", rep_type);
			break;

		case REPETITION_1: /* x-dimension y-dimension x-space y-space */
			fprintf(write_file, "repetition(type=%u ", rep_type);
			U_INT rep1_arr[4];

			for(U_INT i = 0; i < 4; i++)
			{
				byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
				rep1_arr[i] = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);

				// update byte_begin_pos
				byte_begin_pos += byte_num;
			}

			if(unit_select_in)
			{
				fprintf(write_file, "x-dim=%u y-dim=%u x-space=%.3lf y-space=%.3lf)", rep1_arr[0], rep1_arr[1],\
						(DOUBLE) rep1_arr[2] / unit_in, (DOUBLE) rep1_arr[3] / unit_in);
			}
			else
			{
				fprintf(write_file, "x-dim=%u y-dim=%u x-space=%u y-space=%u)", rep1_arr[0], rep1_arr[1], rep1_arr[2], rep1_arr[3]);
			}

			break;

		case REPETITION_2: /* x-dimension x-space */
//			fputs("do not implement repetition 2 yet\n", write_file);
			fprintf(write_file, "repetition(type=%u ", rep_type);

			U_INT rep2_arr[2];

			for(U_INT i = 0; i < 2; i++)
			{
				byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
				rep2_arr[i] = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);

				// update byte_begin_pos
				byte_begin_pos += byte_num;

			}

			if(unit_select_in)
			{
				fprintf(write_file, "x-dim=%u x-space=%.3lf)", rep2_arr[0], (DOUBLE) rep2_arr[1] / unit_in);
			}
			else
			{
				fprintf(write_file, "x-dim=%u x-space=%u)", rep2_arr[0], rep2_arr[1]);
			}

			break;

		case REPETITION_3: /* y-dimension y-space */
			fprintf(write_file, "repetition(type=%u ", rep_type);

			U_INT rep3_arr[2];

			for(U_INT i = 0; i < 2; i++)
			{
				byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
				rep3_arr[i] = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);

				// update byte_begin_pos
				byte_begin_pos += byte_num;

			}

			if(unit_select_in)
			{
				fprintf(write_file, "y-dim=%u y-space=%.3lf)", rep3_arr[0], (DOUBLE) rep3_arr[1] / unit_in);
			}
			else
			{
				fprintf(write_file, "y-dim=%u y-space=%u)", rep3_arr[0], rep3_arr[1]);
			}

			break;

		case REPETITION_4:
			fputs("do not implement repetition 4 yet\n", write_file);
			break;

		case REPETITION_9: /* dimension displacement */
			fprintf(write_file, "repetition(type=%u ", rep_type);

			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			dimension = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
			fprintf(write_file, "dim=%u ", dimension);

			// update byte_begin_pos
			byte_begin_pos += byte_num;

			if(oasis_arr[byte_begin_pos] & 0x01) // gdelta2
			{
				gdelta2 = GetGDelta2(oasis_arr, byte_begin_pos, byte_num);

				if(unit_select_in)
				{
					fprintf(write_file, "disp=g(%.3lf,%.3lf))", (DOUBLE) gdelta2.first_num / unit_in, (DOUBLE) gdelta2.second_num / unit_in);
				}
				else
				{
					fprintf(write_file, "disp=g(%d,%d))", gdelta2.first_num, gdelta2.second_num);
				}

				// update byte_begin_pos
				byte_begin_pos += byte_num;
			}
			else // gdelta1
			{
				printf("[repetition] Do not implement gdelta1 yet\n");
			}

			break;

		case REPETITION_11: /* dimension grid displacement(1)..displacement(P-1) */
			// dump dimension
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			dimension = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);

			// update byte_begin_pos
			byte_begin_pos += byte_num;

			// dump grid
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			grid = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
			fprintf(write_file, "repetition(type=%u dim=%u grid=%u disp=", rep_type, dimension, grid);

			// update byte_begin_pos
			byte_begin_pos += byte_num;

			// dump displacements
			P = dimension + 2;
			for(U_INT i = 1; i < P; i++)
			{
				if(oasis_arr[byte_begin_pos] & 0x01) // gdelta2
				{
					gdelta2 = GetGDelta2(oasis_arr, byte_begin_pos, byte_num);

					if(unit_select_in)
					{
						fprintf(write_file, "g(%.3lf,%.3lf) ", (DOUBLE) (gdelta2.first_num * grid) / unit_in, \
								(DOUBLE) (gdelta2.second_num * grid) / unit_in);
					}
					else
					{
						fprintf(write_file, "g(%d,%d) ", gdelta2.first_num * grid, gdelta2.second_num * grid);
					}

					// update byte_begin_pos
					byte_begin_pos += byte_num;
				}
				else // gdelta1
				{
					gdelta1 = GetGDelta1(oasis_arr, byte_begin_pos, byte_num);
					x += (gdelta1.x * grid);
					y += (gdelta1.y * grid);

					if(unit_select_in)
					{
						fprintf(write_file, "g(%.3lf, %.3lf) ", (DOUBLE) x / unit_in, (DOUBLE) y / unit_in);
					}
					else
					{
						fprintf(write_file, "g(%d, %d) ", x, y);
					}

					// update byte_begin_pos
					byte_begin_pos += byte_num;
				}
			}

			break;

	}

	// calculate rep_length_out
	rep_length_out = byte_begin_pos - rep_begin_pos_in;

	return;
}


VOID DumpPlacement(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL &cell_out, DOUBLE unit_in, BOOL unit_select_in, U_INT &rec_length_out, FILE *write_file)
{
	U_INT byte_num;
	U_INT byte_begin_pos = rec_begin_pos_in + PLACEMENT_ID_LENGTH + 1;

	cell_out.placement.id = oasis_arr[rec_begin_pos_in];
	cell_out.placement.info_byte = oasis_arr[rec_begin_pos_in + PLACEMENT_ID_LENGTH];

	BOOL C = cell_out.placement.info_byte & 0x80;
	BOOL N = cell_out.placement.info_byte & 0x40;
	BOOL X = cell_out.placement.info_byte & 0x20;
	BOOL Y = cell_out.placement.info_byte & 0x10;
	BOOL R = cell_out.placement.info_byte & 0x08;
	BOOL F = cell_out.placement.info_byte & 0x01;

	if(cell_out.placement.id == 17)
	{
		fprintf(write_file, "\n\n%09d\tPLACEMENT id=%u ", rec_begin_pos_in, cell_out.placement.id);
		fprintf(write_file, "CNXYRAAF=%d%d%d%d%d%d%d%d ", C, N, X, Y, R, cell_out.placement.info_byte & 0x04, cell_out.placement.info_byte & 0x02, F);
	}
	else /* placement.id == 18 */
	{
		fprintf(write_file, "\n\nPLACEMENT id=%u ", cell_out.placement.id);
		fprintf(write_file, "CNXYRMAF=%d%d%d%d%d%d%d%d ", C, N, X, Y, R, cell_out.placement.info_byte & 0x04, cell_out.placement.info_byte & 0x02, F);
	}

	// C=1, the cell reference is explicit
	if(C)
	{
		// N=1, reference-number is present, and refers to a CELLNAME record where the cell name is stored
		if(N)
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			cell_out.placement.ref_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
			byte_begin_pos += byte_num;
			fprintf(write_file, "ref_num=%u ", cell_out.placement.ref_num);
		}
		// N=0, cellname-string (n-string) is present and stores the cell name locally
		else
		{
			// in the future
		}
	}
	// C=0, N is ignored, and the value of modal variable placement-cell is used,
	// referring to the same cell as the previous PLACEMENT record
	else
	{

	}

	if(X) // x is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.placement.x = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

		if(unit_select_in)
		{
			fprintf(write_file, "x=%.3lf ", (DOUBLE) cell_out.placement.x / unit_in);
		}
		else
		{
			fprintf(write_file, "x=%d ", cell_out.placement.x);
		}

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}

	if(Y) // y is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.placement.y = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

		if(unit_select_in)
		{
			fprintf(write_file, "y=%.3lf ", (DOUBLE) cell_out.placement.y / unit_in);
		}
		else
		{
			fprintf(write_file, "y=%d ", cell_out.placement.y);
		}

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}

	if(R) // repetition is present
	{
		DumpRepetition(byte_begin_pos, oasis_arr, unit_in, unit_select_in, byte_num, write_file);

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}

	if(F)
	{
		// in the future
	}

	//calculate rec_length_out
	rec_length_out = byte_begin_pos - rec_begin_pos_in;

	return;
}


VOID DumpText(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL &cell_out, DOUBLE unit_in, BOOL unit_select_in, U_INT &rec_length_out, FILE *write_file)
{
	U_INT byte_num;
	U_INT byte_begin_pos = rec_begin_pos_in + TEXT_ID_LENGTH + 1;

	cell_out.text.id = oasis_arr[rec_begin_pos_in];
	cell_out.text.info_byte = oasis_arr[rec_begin_pos_in + TEXT_ID_LENGTH];

	BOOL C = cell_out.text.info_byte & 0x40;
	BOOL N = cell_out.text.info_byte & 0x20;
	BOOL X = cell_out.text.info_byte & 0x10;
	BOOL Y = cell_out.text.info_byte & 0x08;
	BOOL R = cell_out.text.info_byte & 0x04;
	BOOL T = cell_out.text.info_byte & 0x02;
	BOOL L = cell_out.text.info_byte & 0x01;

	fprintf(write_file, "\n\n%09d\tTEXT id=%u ", rec_begin_pos_in, cell_out.text.id);
	fprintf(write_file, "0CNXYRTL=0%d%d%d%d%d%d%d ", C, N, X, Y, R, T, L);

	// C=1, the text reference is explicit
	if(C)
	{
		// N=1, reference-number is present, and refers to a TEXTSTRING record where the cell name is stored
		if(N)
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			cell_out.text.ref_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
			fprintf(write_file, "ref_num=%u ", cell_out.text.ref_num);

			// update byte_begin_pos
			byte_begin_pos += byte_num;
		}
		// N = 0, text-string (an n-string) is present and stores the cell name locally
		else
		{
			// in the future
		}
	}
	// C=0, N is ignored, and the value of modal variable textstring is used instead
	else
	{

	}

	if(L) // L=1, textlayer-number is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.text.textlayer_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "textlayer=%u ", cell_out.text.textlayer_num);

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}
	// L=0, textlayer-number is unspecified, it assumes the value of modal variables textlayer
	else
	{

	}

	if(T) // T=1, texttype-number is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.text.texttype_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "texttype=%u ", cell_out.text.texttype_num);

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}
	// T=0, texttype-number is unspecified, it assumes the value of modal variables texttype
	else
	{

	}

	if(X) // x is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.text.x = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

		if(unit_select_in)
		{
			fprintf(write_file, "x=%.3lf ", (DOUBLE) cell_out.text.x / unit_in);
		}
		else
		{
			fprintf(write_file, "x=%d ", cell_out.text.x);
		}

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}

	if(Y) // y is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.text.y = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);

		if(unit_select_in)
		{
			fprintf(write_file, "y=%.3lf ", (DOUBLE) cell_out.text.y / unit_in);
		}
		else
		{
			fprintf(write_file, "y=%d ", cell_out.text.y);
		}

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}

	if(R) // repetition is present
	{
		DumpRepetition(byte_begin_pos, oasis_arr, unit_in, unit_select_in, byte_num, write_file);

		// update byte_begin_pos
		byte_begin_pos += byte_num;
	}

	//calculate rec_length_out
	rec_length_out = byte_begin_pos - rec_begin_pos_in;

	return;
}


VOID DumpTextString(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	TEXTSTRING textstring;
	(VOID *) memset(&textstring, 0, sizeof(textstring));
	U_INT byte_num;

	textstring.id = (U_CHAR) oasis_arr[rec_begin_pos_in];
	byte_num = GetIntegerByteNum(oasis_arr, rec_begin_pos_in + 1);
	textstring.test_byte_num = GetOasisUINT(byte_num, oasis_arr, rec_begin_pos_in + 1);

	fprintf(write_file, "\n\n%09d\tTEXTSTRING id=%u propname=\"", rec_begin_pos_in, textstring.id);

	for(U_INT i = 0; i < textstring.test_byte_num; i++)
	{
		fprintf(write_file, "%c", oasis_arr[rec_begin_pos_in + 2 + i]);
	}
	fputs("\"", write_file);

	rec_length_out = 2 + textstring.test_byte_num;

	return;
}



