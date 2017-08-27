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

#ifndef __TS_FILTER_H__
#define __TS_FILTER_H__

#include "glob_types.h"
#include "msgQ.h"

#define PSI_INFO_MSGS "PSI_INFO_MSGS"

enum class SectionFilterType {
	PAT,
	PMT,
	MAX
};

enum class FilterStatus {
	FILTER_INITED,
	FILTER_STARTED,
	FILTER_PARSING,
	FILTER_PARSING_DONE,
	FILTER_VERSION_UP,
	FILTER_MAX
};

enum class FilterMessage {
	PAT_PARSING_DONE,
	PAT_VERSION_UP,
	PMT_PARSING_DONE,
	PMT_VERSION_UP
};

class CTsFilter {
public:
	CTsFilter();
	virtual void setFilterStatus(const FilterStatus& stat);
	virtual void setSectionFilterType(const SectionFilterType& stat);
	virtual const FilterStatus& getFilterStatus();
	virtual const SectionFilterType& getSectionFilterType();	
	virtual void notify(const FilterStatus& stat) = 0;
	virtual void parsing(UINT8 *pData) = 0;
protected:
	SectionFilterType type;
	FilterStatus status;
};
#endif