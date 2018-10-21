#ifndef OASISHANDLE_H_
#define OASISHANDLE_H_

#include "limits.h"

typedef unsigned short 			U_SHORT;
typedef short 					S_SHORT;
typedef unsigned int 			U_INT;
typedef int 					S_INT;
typedef unsigned char 			U_CHAR;
typedef char 					S_CHAR;
typedef void 					VOID;
typedef bool 					BOOL;
typedef double 					DOUBLE;

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
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
#define POLYGON_ID_LENGTH			(1)
#define TEXT_ID_LENGTH				(1)
#define PROPERTY_ID_LENGTH			(1)
#define XYRELATIVE_ID_LENGTH		(1)
#define INFO_BYTE_LENGTH			(1)
#define CELL_ID_LENGTH				(1)
#define CELLNAME_ID_LENGTH			(1)
#define ARRAY_MAX					(10 * USHRT_MAX)


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
	PATH_ID,
	PROPERTY28_ID=28,
	PROPERTY29_ID
} RECORD_ID;


typedef struct
{
	U_INT value;
	S_INT x;
	S_INT y;
	U_CHAR dir;
} GDELTA1;


typedef struct
{
	S_INT first_num;
	S_INT second_num;
} GDELTA2;


typedef struct
{
	DOUBLE unit;
	U_CHAR unit_length;
	U_INT length;
	S_CHAR version[4];
	U_CHAR id;
	BOOL offset_flag;
	U_INT unit_begin_pos;
	U_INT offset_flag_begin_pos;
} START;


typedef struct
{

} END;


typedef struct
{
	S_CHAR magic_bytes[19];
	U_INT rec_length;
	U_INT rec_begin_pos;
	S_INT *byte_arr;
	U_INT length;
	BOOL unit_select;
	END end;
	START start;

} OASIS;


VOID GetOasis2Array(S_INT oasis_arr[], U_INT &arr_element_out, const S_CHAR file_name_in[]);
VOID InvertArray(U_CHAR arr[], U_CHAR arr_element_in);
VOID Bin7ToArray(U_INT dec_in, U_CHAR arr[], U_SHORT &arr_element_out);
VOID Bin8For0ToArray(U_CHAR arr[], U_SHORT &arr_element_out);
DOUBLE Exponent2(S_SHORT exponent_in);
DOUBLE GetUnit(S_INT arr_oasis[], U_INT unit_start_pos_in, U_CHAR &unit_length_out);


VOID DumpOasis(OASIS oasis);
VOID DumpStart(START start, S_INT oasis_arr[], U_INT &rec_length);


#endif /* OASISHANDLE_H_ */
