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
#include <iostream>
#include "tsfilterManager.h"

void CTsFilterManager::onInit() {
	inLoop = true;
	attachSectionFilter(0, SectionFilterType::PAT);

	t = std::thread([&]() {
		while(inLoop) {
			eventHandler();
		}
	});
}

void CTsFilterManager::attachSectionFilter(uint32_t pid, const SectionFilterType& type) {
	switch(type) {
		case SectionFilterType::PAT:
			um.emplace(std::make_pair(pid, std::make_shared<CPAT>(type)));
			break;
		case SectionFilterType::PMT:
			break;
		default:
			break;
	}
}

void CTsFilterManager::dispatchPidAndSection(UINT8 *buff) {
	uint32_t pid;

	if(buff[0] != 0x47) {
		std::cout << "Not Header Start " << std::endl;
		return;
	}

	pid = ((buff[1] & 0x1f) << 8) | buff[2];

	auto search = um.find(pid);
	if(search != um.end()) {
		std::cout << "pid found ..." << std::endl;
		search->second->parsing(buff+4);
	}
}

void CTsFilterManager::eventHandler() {
	for(auto u : um) {
		switch(u.second->getFilterStatus()) {
			case FilterStatus::FILTER_INITED:
			{
				// TODO.
			}
			break;
			case FilterStatus::FILTER_STARTED:
			{
				// TODO.
			}
			break;
			case FilterStatus::FILTER_PARSING:
			{
				// TODO.
			}
			break;
			case FilterStatus::FILTER_PARSING_DONE:
			{
				switch(u.second->getSectionFilterType()) {
					case SectionFilterType::PAT:
					{
						// attach PMT filter
						//uint32_t nums 	= static_cast<CPat*>(u.second.get())->getPatProgNum();
						//uint32_t *pids 	= static_cast<CPat*>(u.second.get())->getPmtPids();

						//for(uint32_t i = 0; i < nums; i++) {
						//	attachSectionFilter(pids[i], SectionFilterType::PMT);
						//}
					}
					break;
					case SectionFilterType::PMT:
					{
						// TODO.
					}
					break;
					default:
					break;					
				}
			}
			break;
			case FilterStatus::FILTER_VERSION_UP:
			{
				// TODO.
			}
			break;
			default:
			break;																
		}
	}
}

CTsFilterManager::~CTsFilterManager() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
	inLoop = false;
	t.join();
}