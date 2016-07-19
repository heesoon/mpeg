#ifndef __PMT_H__
#define __PMT_H__

#include <cstdio>
#include <iostream>
#include <memory>
#include <forward_list>
#include "types.h"
#include "psi_glob.h"

/* -------------------------------------------------------------------------------------------------------------------
	declare define
------------------------------------------------------------------------------------------------------------------- */
#define _RESERVED			0x00
#define MPEG1_VIDEO			0x01
#define MPEG2_VIDEO			0x02
#define MPEG1_AUDIO			0x03
#define MPEG2_AUDIO			0x04
#define PRIVATE_SECTION		0x05
#define PES_PRIVATE_DATA	0x06
#define MHEG				0x07
#define DSMCC				0x08
#define MPEG4_VIDEO			0x10
#define MPEG4_H264_VIDEO	0x1B
#define HEVC_VIDEO			0x24

//#define PRINT_PMT_SECSION_HEADER
#ifdef PRINT_PMT_SECSION_HEADER
#define PRINT_PMT_INFO(x) std::printf("%30s = %10d [%#10x]\n", #x, x, x);
#else
#define PRINT_PMT_INFO(x)
#endif

/* -------------------------------------------------------------------------------------------------------------------
	declare class data
------------------------------------------------------------------------------------------------------------------- */
typedef struct {
	UINT8 stream_type;
	UINT16 es_pid;
} ES_INFO_T;

typedef struct {
	UINT8 version;
	UINT8 section_number;
	UINT16 program_number;
	UINT16 pcr_pid;
	std::forward_list<ES_INFO_T> es_info;
} PMT_INFO_T;

class PMT
{
public :
	PMT();
	~PMT();
	RESULT_T parsingPMTSection(UINT16 pmtPid, UINT8* pData);
	bool isExistPmt(UINT8 vserion_number, UINT8 section_number, UINT16 program_number);
	void addNewPMTtoList(PMT_INFO_T newPmt);
	void addNewEStoPMT(PMT_INFO_T newPmt, ES_INFO_T newEs);
	void printPMTList(void);
	UINT32 getPMTProgNum(void);

private :
	UINT32 numProgs;
	std::forward_list<PMT_INFO_T> pmtList;
};

#endif