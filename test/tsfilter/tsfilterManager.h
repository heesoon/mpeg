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

#ifndef __TS_FILTER__MANAGER_H__
#define __TS_FILTER__MANAGER_H__

#include <unordered_map>
#include <memory>
#include <thread>
#include "pat.h"
#include "pmt.h"

class CTsFilterManager {
public:
	CTsFilterManager();
	void init();
	void attachSectionFilter(uint32_t pid, const SectionFilterType& type);
	void detachSectionFilter(const SectionFilterType& type);
	void eventHandler();
	void dispatchPidAndSection(UINT8 *buff);
	virtual ~CTsFilterManager();
private:
	bool inLoop;
	std::thread t;
	std::shared_ptr<CMsgQ<FilterMessage>> pMsgQ;
	std::unordered_map<uint32_t, std::shared_ptr<CTsFilter>> um;
};
#endif