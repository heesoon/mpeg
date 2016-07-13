#ifndef __TYPES_H__
#define __TYPES_H__
#include <iostream>

typedef bool BOOL;
typedef char SINT8;
typedef unsigned char UINT8;
typedef short int SINT16;
typedef unsigned short UINT16;
typedef int SINT32;
typedef unsigned int UINT32;
typedef long long SINT64;
typedef unsigned long long UINT64;

typedef enum 
{
	RESULT_NOT_SUPPORT = -3,
	RESULT_INVALID = -2,
	RESULT_NOTOK = -1,
	RESULT_OK = 0
} RESULT_T;
#endif

