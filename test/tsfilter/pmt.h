/**
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
For more information, please refer to <http://unlicense.org>
*/

#ifndef __PMT_H__
#define __PMT_H__

#include "tsfilter.h"
#include "json_spirit.h"
#include <bitset>
#include <forward_list>

//#define PRINT_PMT_INFO(x) std::printf("%30s = %10d [%#10x]\n", #x, x, x);
#define PRINT_PMT_INFO(x)
#define PMT_HEADER_SIZE 12

struct ES_INFO_T {
	UINT8 stream_type;
	UINT16 es_pid;
};

struct PMT_INFO_T {
	UINT8 version;
	UINT8 section_number;
	UINT16 program_number;
	UINT16 pcr_pid;
	std::forward_list<ES_INFO_T> es_info;
};

class CPMT : public CTsFilter {
public:
	CPMT();
	CPMT(const SectionFilterType& t);
	CPMT(const UINT16& pmtPid, const SectionFilterType& t);
	virtual ~CPMT();
	virtual void addNewPMTtoList(PMT_INFO_T newPmt);
	virtual void addNewEStoPMT(PMT_INFO_T newPmt, ES_INFO_T newEs);
	virtual void printPMTList(void);
	virtual void savePmtToJson(void);
	virtual UINT32 getPMTProgNum(void);	
	virtual bool isExistSection(UINT8 version, UINT8 section_number, UINT16 program_number);
	virtual void notify(const FilterStatus& stat) override;
	virtual void parsing(UINT8 *pData) override;
private:
	UINT32 numProgs;
	UINT16 pmtPid;
	UINT16 program_number;
	UINT8  version = 0xE0;         // lower 5 bit valid
	UINT8  section_count = 0;	
	UINT8  last_section_number = 0;
	std::bitset<256> b;
	std::forward_list<PMT_INFO_T> pmtList;
	std::shared_ptr<CMsgQ<FilterMessage>> pMsgQ;		
};
#endif