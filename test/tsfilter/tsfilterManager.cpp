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
#include <fstream>
#include "tsfilterManager.h"

CTsFilterManager::CTsFilterManager() : isAssigingFilter(false), inLoop(false) {
	init();
}

void CTsFilterManager::init() {
	inLoop = true;

	// pmt result file delete
	if(std::remove("result/pmt_json.txt") != 0) {
		std::cout << "pmt result file delete fail..." << std::endl;
	}

	pMsgQ = CMsgQManager<FilterMessage>::getInstance().createMsgQ(PSI_INFO_MSGS);

	attachSectionFilter(0, SectionFilterType::PAT);

	t = std::thread([&]() {
		while(inLoop) {
			eventHandler();
		}
	});

	t.detach();
}

void CTsFilterManager::attachSectionFilter(uint32_t pid, const SectionFilterType& type) {
	isAssigingFilter = true;

	switch(type) {
		case SectionFilterType::PAT:
			um.emplace(std::make_pair(pid, std::make_shared<CPAT>(pid, type)));
			break;
		case SectionFilterType::PMT:
			um.emplace(std::make_pair(pid, std::make_shared<CPMT>(pid, type)));
			break;
		default:
			break;
	}

	isAssigingFilter = false;
}

void CTsFilterManager::detachSectionFilter(const SectionFilterType& type) {
	for(auto it = um.begin(); it !=um.end();) {
		if(it->second->getSectionFilterType() == type)
			it = um.erase(it);
		else
			++it;
	}
}

void CTsFilterManager::dispatchPidAndSection(UINT8 *buff) {
	UINT32 pid;

	if(isAssigingFilter == true) {
		std::cout << "filter assigning ..." << std::endl;
	}

	if(buff[0] != 0x47) {
		std::cout << "Not Header Start " << std::endl;
		return;
	}

	pid = ((buff[1] & 0x1f) << 8) | buff[2];

	auto search = um.find(pid);
	if(search != um.end()) {

		search->second->parsing(buff+4, pid);
	}
}

void CTsFilterManager::eventHandler() {
	FilterMessage msg = pMsgQ->receiveMsg();
	std::cout << "new message arrived ... " << std::endl;
	switch(msg) {
		case FilterMessage::PAT_PARSING_DONE:
		{
			std::cout << "PAT Parsing Done ..." << std::endl;

			auto pat = um.find(0);
			if(pat != um.end()) {
				std::cout << "number of program : " << static_cast<CPAT*>(pat->second.get())->getNumberOfProgram() << std::endl;
				static_cast<CPAT*>(pat->second.get())->savePatToJson();

				// attach PMT filter
				UINT32 count = 0;
				std::vector<PROGRAM_T> progs = static_cast<CPAT*>(pat->second.get())->getPATInfo();
				for(auto p : progs) {
					attachSectionFilter(p.pmtPid, SectionFilterType::PMT);
				}			
			}
		}
		break;		
		case FilterMessage::PMT_PARSING_DONE:
			std::cout << "PMT Parsing Done ..." << std::endl;
		break;
		case FilterMessage::PAT_VERSION_UP:
		case FilterMessage::PMT_VERSION_UP:
		{
			std::cout << "Table Version Up ..." << std::endl;
			for(auto f : um) {
				f.second->notify(FilterStatus::FILTER_VERSION_UP);
			}
		}
		break;
		default:
		break;
	}
}

CTsFilterManager::~CTsFilterManager() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
	inLoop = false;
	//t.join();
}