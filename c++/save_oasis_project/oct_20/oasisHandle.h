#ifndef OASISHANDLE_H_
#define OASISHANDLE_H_

#include "limits.h"

typedef unsigned short U_SHORT;
typedef short S_SHORT;
typedef unsigned int U_INT;
typedef int S_INT;
typedef unsigned char U_CHAR;
typedef char S_CHAR;
typedef void VOID;
typedef bool BOOL;
typedef double DOUBLE;

#ifndef TRUE
#define TRUE (1)
#endif

#define MAGIC_BYTE_LENGTH 			(13)
#define START_ID_LENGTH 			(1)
#define START_ID_POS 				(13)
#define END_LENGTH 					(256)
#define VERSION_LENGTH 				(4)
#define UNIT_POS 					(18)
#define PROPERTY_ID 				(28)
#define TABLE_OFFSETS_LENGTH 		(12)
#define VALIDATION_SIGN_LENGTH 		(4)
#define OFFSET_FLAG_LENGTH 			(1)
#define DIR_BIT_2_DELTA_LENGTH		(2)
#define SIGN_BIT_LENGTH				(1)
#define TABLE_OFFSETS_ELEMENTS 		(12)
#define END_ID_LENGTH				(1)
#define REPETITION_TYPE_LENGTH		(1)
#define PLACEMENT_ID_LENGTH			(1)
#define RECTANGLE_ID_LENGTH			(1)
#define TEXT_ID_LENGTH				(1)
#define PROPERTY_ID_LENGTH			(1)
#define XYRELATIVE_ID_LENGTH		(1)
#define ARRAY_MAX					(2 * USHRT_MAX)

//define struct OASIS format
typedef struct
{
	S_CHAR magic_bytes[13];
	U_INT start_length;
	S_CHAR version[4];
	BOOL offset_flag;
	U_INT offset_flag_pos;
	U_INT end_begin_pos;
	DOUBLE unit;
	U_INT unit_begin_pos;
	U_CHAR unit_length;
	U_INT table_offsets_begin_pos;
	U_INT table_offsets_last_pos;
	U_INT table_offsets[TABLE_OFFSETS_ELEMENTS];
	U_INT table_offsets_length;
	U_CHAR scheme;
	U_INT scheme_begin_pos;
	U_CHAR scheme_flag;
	U_INT padding_string_length;
	U_INT next_rec_begin_pos;
	U_INT next_rec_length;
	U_INT remain_rec_length;
	U_INT remain_last_pos;

} OASIS;

typedef struct
{
	U_INT pointlist_length;
	U_INT vertex_count;
	U_INT pointlist_begin_pos;
	S_CHAR pointlist_type;
	S_INT x;
	S_INT y;
} POINTLIST;

typedef struct
{
	U_CHAR id;
	U_INT ref_num;
	U_INT byte_num;
	S_CHAR propname_string[USHRT_MAX];
	S_CHAR prop_value_count;
	S_CHAR value_type;
	U_INT property_value[SCHAR_MAX];

} PROPERTY;

//typedef struct
//{
//	U_INT ref_num;
//	U_CHAR id;
//} CELL;

typedef struct
{
	U_CHAR id;
//	BOOL flag;
} XYRELATIVE;


typedef struct
{
	U_CHAR id;
} XYABSOLUTE;


typedef struct
{
	U_CHAR id;
	U_INT layer_num;
	U_INT datatype_num;
	U_INT width;
	U_INT geometry_w;
	U_INT height;
	U_INT geometry_h;
	S_INT x;
	S_INT geometry_x;
	S_INT actual_x;
	S_INT y;
	S_INT geometry_y;
	S_INT actual_y;
	U_CHAR info_byte;
} RECTANGLE;


typedef struct
{
	U_CHAR id;
	U_INT layer_num;
	U_INT datatype_num;
	S_INT x;
	S_INT geometry_x;
	S_INT actual_x;
	S_INT y;
	S_INT geometry_y;
	S_INT actual_y;
	POINTLIST pointlist;
	U_CHAR info_byte;
} POLYGON;


typedef struct
{
	S_CHAR cellname_string[USHRT_MAX];
	U_INT ref_num;
	U_CHAR id;
	S_INT x;
	S_INT y;

} PLACEMENT;


typedef struct
{
	S_CHAR text_string[USHRT_MAX];
	U_INT ref_num;
	U_INT textlayer_num;
	U_INT texttype_num;
	U_CHAR id;
	S_INT x;
	S_INT y;

} TEXT;


typedef struct
{
	U_CHAR id;
	U_INT test_byte_num;
} CELLNAME;

typedef struct
{
	U_CHAR id;
	U_INT test_byte_num;
} PROPNAME;

typedef struct
{
	U_CHAR id;
	U_INT test_byte_num;
} PROPSTRING;


typedef struct
{
	U_CHAR id;
	U_INT test_byte_num;
} TEXTSTRING;


typedef struct
{
	S_INT first_num;
	S_INT second_num;
} GDELTA2;


typedef struct
{
	U_INT ref_num;
	U_CHAR id;
	RECTANGLE rectangle;
	POLYGON polygon;
	PLACEMENT placement;
	XYRELATIVE xyrelative;
	XYABSOLUTE xyabsolute;
	BOOL relative_flag;
} CELL;


typedef enum
{
	PAD_ID,
	START_ID,
	END_ID,
	CELLNAME3_ID,
	CELLNAME4_ID,
	TEXTSTRING5_ID,
	TEXTSTRING6_ID,
	PROPNAME7_ID,
	PROPNAME8_ID,
	PROPSTRING9_ID,
	PROPSTRING10_ID,
	LAYERNAME11_ID,
	LAYERNAME12_ID,
	CELL13_ID,
	CELL14_ID,
	XYABSOLUTE_ID,
	XYRELATIVE_ID,
	PLACEMENT17_ID,
	PLACEMENT18_ID,
	TEXT_ID,
	RECTANGLE_ID,
	POLYGON_ID,
	PROPERTY28_ID=28,
	PROPERTY29_ID
} RECORD_ID;

typedef enum
{
	PROP_REAL0,
	PROP_REAL1,
	PROP_REAL2,
	PROP_REAL3,
	PROP_REAL4,
	PROP_REAL5,
	PROP_REAL6,
	PROP_REAL7,
	PROP_U_INT,
	PROP_S_INT,
	PROP_A_STRING,
	PROP_B_STRING,
	PROP_N_STRING,
	PROP_REF_NUM_A,
	PROP_REF_NUM_B,
	PROP_REF_NUM_N
} PROPERTY_VALUE;


typedef enum
{
	POINTLIST_0,
	POINTLIST_1,
	POINTLIST_2,
	POINTLIST_3,
	POINTLIST_4,
	POINTLIST_5,
} POINTLIST_TYPE;

typedef enum
{
	REPETITION_0,
	REPETITION_1,
	REPETITION_2,
	REPETITION_3,
	REPETITION_4,
	REPETITION_5,
	REPETITION_6,
	REPETITION_7,
	REPETITION_8,
	REPETITION_9,
	REPETITION_10,
	REPETITION_11
} REPETITION_TYPE;

U_INT CalcOasisLength();
VOID CheckValidationScheme(FILE *write_file, U_CHAR scheme_in);
U_INT DumpPaddingString(BOOL offset_flag_in, U_INT table_offsets_length_in);
VOID Bin7ToArray(U_INT dec_in, U_CHAR arr[], U_SHORT &arr_element_out);
DOUBLE Exponent2(S_SHORT exponent_in);
U_INT Bin2Dec(U_CHAR arr[], U_SHORT arr_element_in);

VOID Bin8ToArray(U_INT dec_in, U_CHAR arr[], U_SHORT &arr_element_out);
VOID Bin8For0ToArray(U_CHAR arr[], U_SHORT &arr_element_out);
DOUBLE GetIeee8(const U_CHAR arr[]);
VOID InvertArray(U_CHAR arr[], U_CHAR arr_element_in);

VOID DumpTableOffsets(OASIS oasis_in, S_INT ch_in, U_INT pos_in, FILE *write_file, U_INT i_in, U_INT to_in);
VOID DumpRemainRecord(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length, FILE *write_file);
DOUBLE GetUnit(S_INT arr_oasis[], U_INT unit_start_pos_in, U_CHAR &unit_length_out);
VOID DumpProperty(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpCell(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpXYRelative(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpRectangle(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpCellName(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpPropName(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpPropString(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpPolygon(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpPropValue(S_INT oasis_arr[], S_CHAR value_type_in, U_INT &value_begin_pos_out, FILE *write_file);

U_INT GetTableOffsets(S_INT oasis_arr[], U_INT &table_offsets_pos_out);
U_INT GetIntegerByteNum(S_INT oasis_arr[], U_INT integer_pos_in);
S_INT GetOasisSINT(U_INT byte_num_in, S_INT arr_in[], U_INT pos_in);
U_INT GetOasisUINT(U_INT byte_num_in, S_INT arr_in[], U_INT pos_in);

S_INT Get1Delta(U_INT byte_num_in, S_INT arr_in[], U_INT pos_in);
U_INT Get2Delta(U_INT byte_num_in, S_INT arr_in[], U_INT pos_in);

VOID GetOasis2Array(S_INT oasis_arr[], U_INT &arr_element_out, const S_CHAR file_name_in[]);

U_INT GetTableOffsets(S_INT oasis_arr[], U_INT &table_offsets_pos_out, U_INT &table_offsets_length_out);

VOID WriteOasis2Text(S_INT oasis_arr[], U_INT oasis_length_in, const S_CHAR input_file_in[], const S_CHAR output_file_in[]);

VOID DumpPointList(U_INT pointlist_begin_pos_in, S_INT oasis_arr[], POLYGON &polygon_in, POINTLIST pointlist_arr[], U_INT &pointlist_length);
VOID DumpPointList1(U_INT pointlist_begin_pos_in, S_INT oasis_arr[], POLYGON &polygon_in, POINTLIST pointlist_arr[], U_INT &pointlist_length);

VOID DumpPointList0(U_INT pointlist_begin_pos_in, S_INT oasis_arr[], POLYGON &polygon_in, POINTLIST pointlist_arr[], U_INT &pointlist_length);

GDELTA2 GetGDelta2(S_INT oasis_arr[], U_INT byte_begin_pos, U_INT &rep_length_out);
VOID DumpRepetition(U_INT rep_begin_pos_in, S_INT oasis_arr[], U_INT &rep_length_out, FILE *write_file);

VOID DumpPlacement(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);

VOID DumpText(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);
VOID DumpTextString(U_INT rec_begin_pos_in, S_INT oasis_arr[], U_INT &rec_length_out, FILE *write_file);

//VOID DumpRemainRecord(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL &cell_out, U_INT &rec_length, FILE *write_file);

//VOID DumpRectangle(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL &cell_out, U_INT &rec_length_out, FILE *write_file);
VOID DumpRectangle(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, DOUBLE unit_in, U_INT &rec_length_out, FILE *write_file);


VOID DumpRemainRecord(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, DOUBLE unit_in, U_INT &rec_length, FILE *write_file);

VOID DumpXYRelative(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, U_INT &rec_length_out, FILE *write_file);

VOID DumpPolygon(U_INT rec_begin_pos_in, S_INT oasis_arr[], CELL & cell_out, DOUBLE unit_in, U_INT &rec_length_out, FILE *write_file);

#endif /* OASISHANDLE_H_ */
