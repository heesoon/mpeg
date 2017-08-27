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

CTsFilterManager::CTsFilterManager() {
	init();
}

void CTsFilterManager::init() {
	inLoop = true;

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

void CTsFilterManager::detachSectionFilter(const SectionFilterType& type) {
	for(auto it = um.begin(); it !=um.end();) {
		if(it->second->getSectionFilterType() == type)
			it = um.erase(it);
		else
			++it;
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
		search->second->parsing(buff+4);
	}
}

void CTsFilterManager::eventHandler() {
	FilterMessage msg = pMsgQ->receiveMsg();
	switch(msg) {
		case FilterMessage::PAT_PARSING_DONE:
		{
			std::cout << "PAT Parsing Done ..." << std::endl;

#if 0			
			auto pat = um.find(0);
			if(pat != um.end()) {
				std::ofstream ofs ("pat_result.txt");

				UINT32 i = 1;
				std::vector<PROGRAM_T>& progs = static_cast<CPAT*>(pat->second.get())->getPATInfo();
				
				for(auto& p : progs) {
					ofs << "index : " << i++ << std::endl;
					ofs << "version : " << p.version << std::endl;
					ofs << "section_number : " << p.section_number << std::endl;
					ofs << "program_number : " << p.program_number << std::endl;
					ofs << "pmtPid : " << p.pmtPid << std::endl;
				}
				ofs.close();
			}
#endif	
			auto pat = um.find(0);
			if(pat != um.end()) {
				static_cast<CPAT*>(pat->second.get())->savePatToJson();
			}		
		}
		break;
		case FilterMessage::PMT_PARSING_DONE:
			// TODO.
		break;		
		default:
		break;
	}
}

CTsFilterManager::~CTsFilterManager() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
	inLoop = false;
}