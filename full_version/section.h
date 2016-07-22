#ifndef __SECTION_H__
#define __SECTION_H__

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "types.h"

/* -------------------------------------------------------------------------------------------------------------------
	declare define
------------------------------------------------------------------------------------------------------------------- */
//#define PRINT_SECTION
#ifdef PRINT_SECTION
#define PRINT_SECTION_INFO(x) std::printf("%3s = %10d [%#10x]\n", #x, x, x);
#else
#define PRINT_SECTION_INFO(x)
#endif

/* -------------------------------------------------------------------------------------------------------------------
	declare class data
------------------------------------------------------------------------------------------------------------------- */
typedef struct {
	UINT16 pid;
	UINT8 section[188];
} SECTION_INFO_T;

typedef enum {
	MEDIA_TYPE_FILE,
	MEDIA_TYPE_SOCKET,
	MEDIA_TYPE_NOTSUPPORT
} MEDIA_TYPE_T;

class CSection
{	
public :
	CSection();
	~CSection();
	MEDIA_TYPE_T getCurrentMediaType(void);
	RESULT_T openMedia(std::string media);
	RESULT_T readMedia(SECTION_INFO_T* sectionInfo);
	RESULT_T readMediaFromFile(SECTION_INFO_T* sectionInfo);
	
private :
	std::ifstream fin;
	std::string mediaInPath;
	MEDIA_TYPE_T currentOpenedMediaType;
};
#endif