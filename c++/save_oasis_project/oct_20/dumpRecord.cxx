#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "oasisHandle.h"


VOID DumpProperty(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	U_INT byte_begin_pos = rec_begin_pos_in;
	U_INT byte_num;
//	rec_length_out = 0;
	U_CHAR buffer[CHAR_BIT];
	U_SHORT arr_element = 0;
	PROPERTY property;
	memset(&property, 0, sizeof(property));

	Bin8ToArray(oasis_arr[byte_begin_pos + PROPERTY_ID_LENGTH], buffer, arr_element);
	InvertArray(buffer, arr_element);

	U_CHAR VVVV = 0;
	for(U_CHAR i = 0; i < 4; i++)
	{
		VVVV += (U_CHAR) (buffer[i] * Exponent2(4 - i - 1));
	}

	BOOL V = buffer[4];
	BOOL C = buffer[5];
	BOOL N = buffer[6];

	fprintf(write_file, "\n\n%09d\tPROPERTY id=%u ", byte_begin_pos, oasis_arr[byte_begin_pos]);
//	fprintf(write_file, "\n\nPROPERTY id=%u ", oasis_arr[byte_begin_pos]);
	fputs("UUUUVCNS=", write_file);
	for(U_SHORT i = 0; i < arr_element; i++)
	{
		fprintf(write_file, "%u", buffer[i]);
	}
	fputs(" ", write_file);

	if(C)
	{
		if(N) // C = 1, N = 1
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos + 2);
			property.ref_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos + 2);
			fprintf(write_file, "ref_num=%u ", property.ref_num);
			byte_begin_pos += (byte_num + PROPERTY_ID_LENGTH + 1);
		}
		else // C = 1, N = 0
		{
			// in the future
		}
	}

	// V = 1
	// UUUU must be 0, and modal variable last-value-list supplies the value list
	if(V)
	{
		fputs("(same ref_num)", write_file);
		byte_begin_pos += 2;
	}
	else // V = 0
	{
		if(VVVV == 15)
		{
			property.prop_value_count = oasis_arr[byte_begin_pos];

		}
		else
		{
			property.prop_value_count = VVVV;
			fprintf(write_file, "value_count=%u ", property.prop_value_count);

//			byte_begin_pos += byte_num + 2;

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

	cell.id = oasis_arr[rec_begin_pos_in];
	cell.ref_num = oasis_arr[rec_begin_pos_in + 1];

	fprintf(write_file, "\n\n%09d\tCELL id=%u ref_num=%u", rec_begin_pos_in, cell.id, cell.ref_num);
//	fprintf(write_file, "\n\nCELL id=%u ref_num=%u", cell.id, cell.ref_num);

	rec_length_out = 2;

	return;
}


VOID DumpXYRelative(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, U_INT &rec_length_out, FILE *write_file)
{
//	XYRELATIVE xyrelative;
//	(VOID *) memset(&xyrelative, 0, sizeof(xyrelative));

	cell_out.xyrelative.id = oasis_arr[rec_begin_pos_in];

	fprintf(write_file, "\n\n%09d\tXYRELATIVE id=%u", rec_begin_pos_in, cell_out.xyrelative.id);
//	fprintf(write_file, "\n\nXYRELATIVE id=%u", xyrelative.id);

	cell_out.relative_flag = TRUE;

	rec_length_out = XYRELATIVE_ID_LENGTH;

	return;
}


VOID DumpRectangle(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, DOUBLE unit_in, U_INT &rec_length_out, FILE *write_file)
{
	U_CHAR buffer[CHAR_BIT];
	U_SHORT arr_element = 0;
	U_INT byte_num;
	U_INT byte_begin_pos = rec_begin_pos_in + 2;

	cell_out.rectangle.id = oasis_arr[rec_begin_pos_in];

	Bin8ToArray(oasis_arr[rec_begin_pos_in + RECTANGLE_ID_LENGTH], buffer, arr_element);
	InvertArray(buffer, arr_element);

	BOOL S = buffer[0];
	BOOL W = buffer[1];
	BOOL H = buffer[2];
	BOOL X = buffer[3];
	BOOL Y = buffer[4];
	BOOL R = buffer[5];
	BOOL D = buffer[6];
	BOOL L = buffer[7];

	fprintf(write_file, "\n\n%09d\tRECTANGLE id=%u ", rec_begin_pos_in, cell_out.rectangle.id);
//	fprintf(write_file, "\n\nRECTANGLE id=%u ", oasis_arr[rec_begin_pos_in]);

	fputs("SWHXYRDL=", write_file);
	for(U_SHORT i = 0; i < arr_element; i++)
	{
		fprintf(write_file, "%u", buffer[i]);
	}
	fputs(" ", write_file);

	if(L)
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.rectangle.layer_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "layer=%u ", cell_out.rectangle.layer_num);
		byte_begin_pos += byte_num;
	}

	if(D)
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.rectangle.datatype_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "datatype=%u ", cell_out.rectangle.datatype_num);
		byte_begin_pos += byte_num;
	}

	if(S) // rectangle is square
	{
		if(W) // width is present
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			cell_out.rectangle.width = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
//			fprintf(write_file, " width=%u ", cell_out.rectangle.width);
			fprintf(write_file, "(square) width=%.3lf ", (DOUBLE) cell_out.rectangle.width / unit_in);
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
//			fprintf(write_file, " width=%u ", cell_out.rectangle.width);
			fprintf(write_file, " width=%.3lf ", (DOUBLE) cell_out.rectangle.width / unit_in);
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
//			fprintf(write_file, " height=%u ", cell_out.rectangle.height);
			fprintf(write_file, " height=%.3lf ", (DOUBLE) cell_out.rectangle.height / unit_in);
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
//		fprintf(write_file, " x=%d ", cell_out.rectangle.x);
		fprintf(write_file, " x=%.3lf ", (DOUBLE) cell_out.rectangle.x / unit_in);
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
		fprintf(write_file, " y=%.3lf ", (DOUBLE) cell_out.rectangle.y / unit_in);
//		fprintf(write_file, " y=%d ", cell_out.rectangle.y);
		byte_begin_pos += byte_num;
	}
	// y is unspecified, the value of modal variable geometry-y, respectively, is used instead
	else
	{
		cell_out.rectangle.y = 0;
	}

	if(R) // repetition is present
	{
		DumpRepetition(byte_begin_pos, oasis_arr, byte_num, write_file);
		byte_begin_pos += byte_num;
	}

	// in relative mode
	if(cell_out.relative_flag)
	{
		// calculate actual (x, y) coordinate
		cell_out.rectangle.actual_x = cell_out.rectangle.x + cell_out.rectangle.geometry_x;
		cell_out.rectangle.actual_y = cell_out.rectangle.y + cell_out.rectangle.geometry_y;

		fprintf(write_file, " actual_xy=(%.3lf,%.3lf)", (DOUBLE) cell_out.rectangle.actual_x / unit_in, \
				(DOUBLE) cell_out.rectangle.actual_y / unit_in);
	}
	// in absolute mode
	else
	{
		// calculate actual (x, y) coordinate
		cell_out.rectangle.actual_x = cell_out.rectangle.x;
		cell_out.rectangle.actual_y = cell_out.rectangle.y;

		fprintf(write_file, " actual_xy=(%.3lf,%.3lf)", (DOUBLE) cell_out.rectangle.actual_x / unit_in, \
				(DOUBLE) cell_out.rectangle.actual_y / unit_in);
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
//	fprintf(write_file, "\n\nCELLNAME id=%u cellname=\"", cellname.id);

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
//	fprintf(write_file, "\n\nPROPNAME id=%u propname=\"", propname.id);

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
//	fprintf(write_file, "\n\nPROPSTRING id=%u propstring=\"", propstring.id);

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


VOID DumpPolygon(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, DOUBLE unit_in, U_INT &rec_length_out, FILE *write_file)
{
	U_CHAR buffer[CHAR_BIT];
	POINTLIST pointlist_arr[UCHAR_MAX];

	for(U_CHAR i = 0; i < UCHAR_MAX; i++)
	{
		(VOID *) memset(&pointlist_arr[i], 0, sizeof(pointlist_arr[i]));
	}

	U_SHORT arr_element = 0;
	U_INT byte_num;
	U_INT byte_begin_pos = rec_begin_pos_in + 2;

	Bin8ToArray(oasis_arr[rec_begin_pos_in + 1], buffer, arr_element);
	InvertArray(buffer, arr_element);

	BOOL P = buffer[2];
	BOOL X = buffer[3];
	BOOL Y = buffer[4];
	BOOL R = buffer[5];
	BOOL D = buffer[6];
	BOOL L = buffer[7];

	fprintf(write_file, "\n\n%09d\tPOLYGON id=%u ", rec_begin_pos_in, oasis_arr[rec_begin_pos_in]);
//	fprintf(write_file, "\n\nPOLYGON id=%u ", oasis_arr[rec_begin_pos_in]);

	fputs("00PXYRDL=", write_file);

	for(U_SHORT i = 0; i < arr_element; i++)
	{
		fprintf(write_file, "%u", buffer[i]);
	}
	fputs(" ", write_file);

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
		fprintf(write_file, "pointlist(type=%u vertex_count=%u xy=[", cell_out.polygon.pointlist.pointlist_type, cell_out.polygon.pointlist.vertex_count);

		for(U_INT i = 0; i < cell_out.polygon.pointlist.vertex_count; i++)
		{
			if(i == cell_out.polygon.pointlist.vertex_count - 1)
			{
				fprintf(write_file, "(%.3lf,%.3lf)] ", (DOUBLE) pointlist_arr[i].x / unit_in, (DOUBLE) pointlist_arr[i].y / unit_in);
			}
			else
			{
				fprintf(write_file, "(%.3lf,%.3lf) ", (DOUBLE) pointlist_arr[i].x / unit_in, (DOUBLE) pointlist_arr[i].y / unit_in);
			}

		}

		byte_begin_pos += cell_out.polygon.pointlist.pointlist_length;
	}

	if(X) // x is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		cell_out.polygon.x = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);
//		fprintf(write_file, "x=%d ", cell_out.polygon.x);
		fprintf(write_file, "x=%.3lf ", (DOUBLE) cell_out.polygon.x / unit_in);
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
//		fprintf(write_file, "y=%d ", cell_out.polygon.y);
		fprintf(write_file, "y=%.3lf ", (DOUBLE) cell_out.polygon.y / unit_in);
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

		fprintf(write_file, " initial_vertex=(%.3lf,%.3lf)", (DOUBLE) cell_out.polygon.actual_x / unit_in, \
				(DOUBLE) cell_out.polygon.actual_y / unit_in);
	}
	// in absolute mode
	// in this mode, explicit x and y values, when present, are used directly as the actual (x,y) coordinates
	else
	{
		// calculate actual (x, y) coordinate
		cell_out.polygon.actual_x = cell_out.polygon.x;
		cell_out.polygon.actual_y = cell_out.polygon.y;

		fprintf(write_file, " initial_vertex=(%.3lf,%.3lf)", (DOUBLE) cell_out.polygon.actual_x / unit_in, \
				(DOUBLE) cell_out.polygon.actual_y / unit_in);
	}

	// show actual vertex of polygon
	fputs(" actual_vertex=[", write_file);
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

		byte_pos += byte_num;

		pointlist_arr[i].x = polygon_out.pointlist.x;
		pointlist_arr[i].y = polygon_out.pointlist.y;
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

	//calculate the vertex count
	polygon_out.pointlist.vertex_count = GetOasisUINT(byte_num, oasis_arr, byte_pos);

	//update the byte position
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

		byte_pos += byte_num;

		pointlist_arr[i].x = polygon_out.pointlist.x;
		pointlist_arr[i].y = polygon_out.pointlist.y;
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


VOID DumpRepetition(U_INT rep_begin_pos_in, S_INT oasis_arr[], U_INT &rep_length_out, FILE *write_file)
{
	U_INT rep_type = oasis_arr[rep_begin_pos_in];
	U_INT dimension;
	U_INT byte_num;
	GDELTA2 gdelta2;

	rep_length_out = REPETITION_TYPE_LENGTH;

	switch(rep_type)
	{
		case REPETITION_0: /* re-use the previous repetition definition */
			fprintf(write_file, "repetition(type=%u)", rep_type);
			break;

		case REPETITION_1: /* x-dimension y-dimension x-space y-space */
			fprintf(write_file, "repetition(type=%u ", rep_type);
			U_INT rep1_arr[4];

			for(U_INT i = 0; i < 4; i++)
			{
				byte_num = GetIntegerByteNum(oasis_arr, rep_begin_pos_in + REPETITION_TYPE_LENGTH);
				rep1_arr[i] = GetOasisUINT(byte_num, oasis_arr, rep_begin_pos_in + REPETITION_TYPE_LENGTH);

				//update rep_begin_pos_in
				rep_begin_pos_in += byte_num;

				// update rep_length_out
				rep_length_out += byte_num;
			}

			fprintf(write_file, "x-dim=%u y-dim=%u x-space=%u y-space=%u)", rep1_arr[0], rep1_arr[1], rep1_arr[2], rep1_arr[3]);

			break;

		case REPETITION_3: /* y-dimension y-space */
			fprintf(write_file, "repetition(type=%u ", rep_type);

			U_INT rep3_arr[2];

			for(U_INT i = 0; i < 2; i++)
			{
				byte_num = GetIntegerByteNum(oasis_arr, rep_begin_pos_in + REPETITION_TYPE_LENGTH);
				rep3_arr[i] = GetOasisUINT(byte_num, oasis_arr, rep_begin_pos_in + REPETITION_TYPE_LENGTH);

				//update rep_begin_pos_in
				rep_begin_pos_in += byte_num;

				// update rep_length_out
				rep_length_out += byte_num;
			}

			fprintf(write_file, "y-dim=%u y-space=%u)", rep3_arr[0], rep3_arr[1]);
			break;

		case REPETITION_9: /* dimension displacement */
			fprintf(write_file, "repetition(type=%u ", rep_type);

			byte_num = GetIntegerByteNum(oasis_arr, rep_begin_pos_in + REPETITION_TYPE_LENGTH);
			dimension = GetOasisUINT(byte_num, oasis_arr, rep_begin_pos_in + REPETITION_TYPE_LENGTH);
			fprintf(write_file, "dim=%u ", dimension);

			// update the rep_begin_pos_in
			rep_begin_pos_in += (byte_num + REPETITION_TYPE_LENGTH);

			// update rep_length_out
			rep_length_out += byte_num;

			if(oasis_arr[rep_begin_pos_in] & 0x01) // gdelta2
			{
				gdelta2 = GetGDelta2(oasis_arr, rep_begin_pos_in, rep_length_out);
				fprintf(write_file, "disp=g(%d,%d))", gdelta2.first_num, gdelta2.second_num);
			}
			else // gdelta1
			{
				printf("[repetition] Do not implement gdelta1 yet\n");
			}

			break;

	}

	return;
}


VOID DumpPlacement(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	PLACEMENT placement;
	(VOID *) memset(&placement, 0, sizeof(placement));
	U_SHORT arr_element = 0;
	U_INT byte_num;
	U_CHAR buffer[CHAR_BIT];
	U_INT byte_begin_pos = rec_begin_pos_in + PLACEMENT_ID_LENGTH + 1;

	placement.id = oasis_arr[rec_begin_pos_in];

	Bin8ToArray(oasis_arr[rec_begin_pos_in + 1], buffer, arr_element);
	InvertArray(buffer, arr_element);

	BOOL C = buffer[0];
	BOOL N = buffer[1];
	BOOL X = buffer[2];
	BOOL Y = buffer[3];
	BOOL R = buffer[4];
	BOOL F = buffer[7];

	if(placement.id == 17)
	{
		fprintf(write_file, "\n\n%09d\tPLACEMENT id=%u ", rec_begin_pos_in, placement.id);
//		fprintf(write_file, "\n\nPLACEMENT id=%u ", placement.id);

		fputs("CNXYRAAF=", write_file);
		for(U_SHORT i = 0; i < arr_element; i++)
		{
			fprintf(write_file, "%u", buffer[i]);
		}
	}
	else /* placement.id == 18 */
	{
		fprintf(write_file, "\n\nPLACEMENT id=%u ", placement.id);
		fputs("CNXYRMAF=", write_file);
		for(U_SHORT i = 0; i < arr_element; i++)
		{
			fprintf(write_file, "%u", buffer[i]);
		}
	}
	fputs(" ", write_file);

	if(C)
	{
		// N = 1, reference-number is present, and refers to a CELLNAME record where the cell name is stored
		if(N)
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			placement.ref_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
			byte_begin_pos += byte_num;
			fprintf(write_file, "ref_num=%u ", placement.ref_num);
		}
		// N = 0, cellname-string (an n-string) is present and stores the cell name locally
		else
		{
			// in the future
		}
	}

	if(X) // x is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		placement.x = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "x=%d ", placement.x);
		byte_begin_pos += byte_num;
	}

	if(Y) // y is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		placement.y = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "y=%d ", placement.y);
		byte_begin_pos += byte_num;
	}

	if(R) // repetition is present
	{
		DumpRepetition(byte_begin_pos, oasis_arr, byte_num, write_file);
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


VOID DumpText(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file)
{
	TEXT text;
	(VOID *) memset(&text, 0, sizeof(text));

	U_SHORT arr_element = 0;
	U_INT byte_num;
	U_CHAR buffer[CHAR_BIT];
	U_INT byte_begin_pos = rec_begin_pos_in + TEXT_ID_LENGTH + 1;

	text.id = oasis_arr[rec_begin_pos_in];
	Bin8ToArray(oasis_arr[rec_begin_pos_in + 1], buffer, arr_element);
	InvertArray(buffer, arr_element);

	BOOL C = buffer[1];
	BOOL N = buffer[2];
	BOOL X = buffer[3];
	BOOL Y = buffer[4];
	BOOL R = buffer[5];
	BOOL T = buffer[6];
	BOOL L = buffer[7];

	fprintf(write_file, "\n\n%09d\tTEXT id=%u ", rec_begin_pos_in, text.id);
//	fprintf(write_file, "\n\nTEXT id=%u ", text.id);

	fputs("0CNXYRTL=", write_file);
	for(U_SHORT i = 0; i < arr_element; i++)
	{
		fprintf(write_file, "%u", buffer[i]);
	}
	fputs(" ", write_file);

	if(C)
	{
		// N = 1, reference-number is present, and refers to a TEXTSTRING record where the cell name is stored
		if(N)
		{
			byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
			text.ref_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
			byte_begin_pos += byte_num;
			fprintf(write_file, "ref_num=%u ", text.ref_num);
		}
		// N = 0, text-string (an n-string) is present and stores the cell name locally
		else
		{
			// in the future
		}
	}

	if(L) // textlayer-number is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		text.textlayer_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		byte_begin_pos += byte_num;
		fprintf(write_file, "textlayer=%u ", text.textlayer_num);
	}

	if(T) // texttype-number is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		text.texttype_num = GetOasisUINT(byte_num, oasis_arr, byte_begin_pos);
		byte_begin_pos += byte_num;
		fprintf(write_file, "texttype=%u ", text.texttype_num);
	}

	if(X) // x is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		text.x = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "x=%d ", text.x);
		byte_begin_pos += byte_num;
	}

	if(Y) // y is present
	{
		byte_num = GetIntegerByteNum(oasis_arr, byte_begin_pos);
		text.y = GetOasisSINT(byte_num, oasis_arr, byte_begin_pos);
		fprintf(write_file, "y=%d ", text.y);
		byte_begin_pos += byte_num;
	}

	if(R) // repetition is present
	{
		DumpRepetition(byte_begin_pos, oasis_arr, byte_num, write_file);
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
//	fprintf(write_file, "\n\nTEXTSTRING id=%u propname=\"", textstring.id);

	for(U_INT i = 0; i < textstring.test_byte_num; i++)
	{
		fprintf(write_file, "%c", oasis_arr[rec_begin_pos_in + 2 + i]);
	}
	fputs("\"", write_file);

	rec_length_out = 2 + textstring.test_byte_num;

	return;
}



