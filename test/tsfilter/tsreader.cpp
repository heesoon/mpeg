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
#include "tsreader.h"

// implementation for CTsReader4File
void CTsReader4File::openMedia() {
	std::string fpath;

	std::cout << "enter ts file path : ";
	//std::cin >> fpath;
	std::getline(std::cin, fpath);
	if(fpath.empty()) {
		//fpath = "/home/hskim/stream/secureMedia/fantasy_clear.ts";
		//fpath = "/home/hskim/mpeg/test/stream/test.ts";
		fpath = "../stream/test.ts";
	}
	std::cout << std::endl;

	ifs.open(fpath, std::ifstream::in|std::ifstream::binary);
	if(!ifs.is_open()) {
		std::cout << "file open fail..." << std::endl;
		return;
	}

	//filterMgr = new CTsFilterManager;
	upFilterMgr = std::make_unique<CTsFilterManager>();
	//upFilterMgr->init();
	std::cout << "Success file open.." << std::endl;
}

void CTsReader4File::readMedia() {
	char *buffer = new char[188];
	while(!ifs.eof()) {
		ifs.read(buffer, 188);
		upFilterMgr->dispatchPidAndSection((UINT8*)buffer);
	}
	delete [] buffer;
}

void CTsReader4File::closeMedia() {
	ifs.close();
	//delete filterMgr;
}

CTsReader4File::~CTsReader4File() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
	closeMedia();
}

// implementation for CTsReader4IP
void CTsReader4IP::openMedia() {
	// TODO.
}

void CTsReader4IP::readMedia() {
	// TODO.
}

void CTsReader4IP::closeMedia() {
	// TODO.
}

CTsReader4IP::~CTsReader4IP() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;	
}

// implementation for CTsReader4 Factory
#if 0
CTsReader* CTsReaderFactory::createNewTsReader(const MediaType& type) {
	if(type == MediaType::MEDIA_TYPE_FILE)
		return new CTsReader4File;
	if(type == MediaType::MEDIA_TYPE_IP)
		return new CTsReader4IP;

	return nullptr;
}
#else
std::unique_ptr<CTsReader> CTsReaderFactory::createNewTsReader(const MediaType& type) {
	if(type == MediaType::MEDIA_TYPE_FILE)
		return std::make_unique<CTsReader4File>();
	if(type == MediaType::MEDIA_TYPE_IP)
		return std::make_unique<CTsReader4IP>();

	return nullptr;
}
#endif
