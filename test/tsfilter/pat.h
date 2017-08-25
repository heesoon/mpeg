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

#ifndef __PAT_H__
#define __PAT_H__

#include <vector>
#include <bitset>
#include "tsfilter.h"

struct PROGRAM_T {
	UINT8  version;
	UINT8  section_number;
	UINT16 program_number;
	UINT16 pmtPid;
};

class CPAT : public CTsFilter {
public:
	CPAT();
	CPAT(const SectionFilterType& t);
	virtual UINT32 getNumberOfProgram();
	virtual std::vector<PROGRAM_T>& getPATInfo();
	virtual void notify(const FilterStatus& stat) override;
	virtual bool isExistSection(UINT8 version, UINT8 section_number);	
	virtual void parsing(UINT8 *pData) override;
	virtual ~CPAT();
private:
	UINT32 numProgs = 0;
	UINT8  version = 0xE0;         // lower 5 bit valid
	UINT8  section_count = 0;
	UINT8  last_section_number = 0;
	std::bitset<256> b;
	std::vector<PROGRAM_T> v;
};
#endif