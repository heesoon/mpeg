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
#include "tsfilter.h"

// implementation for CTsFilter4File
void CTsFilter4File::openMedia() {
	std::string fpath;

	std::cout << "enter ts file path : ";
	//std::cin >> fpath;
	std::getline(std::cin, fpath);
	if(fpath.empty()) {
		fpath = "/home/hskim/stream/secureMedia/fantasy_clear.ts";
	}
	std::cout << std::endl;

	ifs.open(fpath, std::ifstream::in|std::ifstream::binary);
	if(!ifs.is_open()) {
		std::cout << "file open fail..." << std::endl;
		return;
	}

	std::cout << "Success file open.." << std::endl;
}

void CTsFilter4File::readMedia() {
	char *buffer = new char[188];
	while(!ifs.eof()) {
		ifs.read(buffer, 188);
		//std::printf("%x ", buffer[0]);
	}
	delete [] buffer;
}

void CTsFilter4File::closeMedia() {
	ifs.close();
}

CTsFilter4File::~CTsFilter4File() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
	closeMedia();
}

// implementation for CTsFilter4IP
void CTsFilter4IP::openMedia() {
	// TODO.
}

void CTsFilter4IP::readMedia() {
	// TODO.
}

void CTsFilter4IP::closeMedia() {
	// TODO.
}

CTsFilter4IP::~CTsFilter4IP() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;	
}

// implementation for CTsFilter Factory
#if 0
CTsFilter* CTsFilterFactory::createNewTsFilter(const MediaType& type) {
	if(type == MediaType::MEDIA_TYPE_FILE)
		return new CTsFilter4File;
	if(type == MediaType::MEDIA_TYPE_IP)
		return new CTsFilter4IP;

	return nullptr;
}
#else
std::unique_ptr<CTsFilter> CTsFilterFactory::createNewTsFilter(const MediaType& type) {
	if(type == MediaType::MEDIA_TYPE_FILE)
		return std::make_unique<CTsFilter4File>();
	if(type == MediaType::MEDIA_TYPE_IP)
		return std::make_unique<CTsFilter4IP>();

	return nullptr;
}
#endif