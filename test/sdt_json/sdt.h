#ifndef __SDT_H__
#define __SDT_H__

#include <cstdio>
#include <iostream>
#include <memory>
#include <forward_list>
#include <fstream>
#include <cassert>
#include "types.h"
#include "psi_glob.h"
#include "json_spirit.h"

/* -------------------------------------------------------------------------------------------------------------------
	declare define
------------------------------------------------------------------------------------------------------------------- */
#define SDT_HEADER_SIZE		11

//#define PRINT_SDT_SECSION_HEADER
#ifdef PRINT_SDT_SECSION_HEADER
#define PRINT_SDT_INFO(x) std::printf("%30s = %10d [%#10x]\n", #x, x, x);
#else
#define PRINT_SDT_INFO(x)
#endif

#define SDT_ACTUAL_PID			0x11
#define SDT_ACTUAL_TABLE_ID		0x42 
/* -------------------------------------------------------------------------------------------------------------------
	declare class data
------------------------------------------------------------------------------------------------------------------- */
typedef struct
{
	UINT8 version;
	UINT8 section_number;
	UINT16 program_number;
	bool EIT_schedule_flag;
	bool EIT_present_following_flag;
	UINT8 running_status;
	bool free_CA_mode;
	std::string servicProvideeName;
	std::string serviceName;
} SDT_INFO_T;

class SDT
{

public :
	SDT();
	~SDT();
	RESULT_T parsingSDTSection(UINT8* pData);
	bool isSDTSecton(UINT16 pid);
	bool isExistSdt(UINT8 version_number, UINT8 section_number, UINT16 program_number);
	void printSDTList(void);
	void saveSdtToJson(void);
	RESULT_T parsingServiceDescriptor(SDT_INFO_T* sdtInfo, UINT8 *pData);

private :
	UINT32 NumOfService;
	std::forward_list<SDT_INFO_T> sdt_info_list;
};

#endif