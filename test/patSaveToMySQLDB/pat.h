#ifndef __PAT_H__
#define __PAT_H__

#include <cstdio>
#include <iostream>
#include <memory>
#include <forward_list>
#include "types.h"
#include "psi_glob.h"

/* -------------------------------------------------------------------------------------------------------------------
	declare define
------------------------------------------------------------------------------------------------------------------- */
#define PAT_HEADER_SIZE 	8

//#define PRINT_PAT_HEADER
#ifdef PRINT_PAT_HEADER
#define PRINT_PAT_HEADER_INFO(x) std::printf("%30s = %10d [%#10x]\n", #x, x, x);
#else
#define PRINT_PAT_HEADER_INFO(x)
#endif

/* -------------------------------------------------------------------------------------------------------------------
	declare class data
------------------------------------------------------------------------------------------------------------------- */
typedef struct {
	UINT8 version;
	UINT8 section_number;
    UINT16 program_number;
    UINT16 pmtPid;
}PROGRAM_T;

class PAT
{
private :
	UINT32 numProgs;
	std::forward_list<PROGRAM_T> patList;

public :
	PAT();
	~PAT();
	bool isPmtPid(UINT16 pmtPid);
	bool isExistPat(UINT8 version_number, UINT8 section_number);
	void printPATList(void);
	RESULT_T parsingPATSection(UINT8 *pData);
	UINT32 getPATProgNum(void);
	std::forward_list<PROGRAM_T>& getPatList(void);
};

#endif
