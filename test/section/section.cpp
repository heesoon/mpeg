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

/** --------------------------------------------------------------------
	file : section.cpp
	get section raw data from TS stream

	version : 1.0
	data    : 2016.06.01
	author  : HeeSoon Kim(chipmaker0304@gmail.com)
-----------------------------------------------------------------------*/
#include "section.h"

CSection::CSection()
{
	currentOpenedMediaType = MEDIA_TYPE_NOTSUPPORT;
	std::cout << __FUNCTION__ << std::endl;
}

CSection::~CSection()
{
	if(currentOpenedMediaType == MEDIA_TYPE_FILE)
	{
		fin.close();
	}
	
	std::cout << __FUNCTION__ << std::endl;
}

MEDIA_TYPE_T CSection::getCurrentMediaType(void)
{
	return currentOpenedMediaType;
}

RESULT_T CSection::openMedia(std::string media)
{
	if(media.compare(0, std::strlen("file://"), "file://") == 0)
	{
		currentOpenedMediaType = MEDIA_TYPE_FILE;
		mediaInPath = media;
		mediaInPath.erase(0, std::strlen("file://"));
		std::cout << "current media type : FILE " << std::endl;

		fin.open(mediaInPath.c_str(), std::ios_base::in | std::ios_base::binary);

		if(fin.is_open() == false)
		{
			std::cout << "File Open Error" << std::endl;
			return RESULT_NOTOK;			
		}
	}
	else if(media.compare(0, std::strlen("udp://"), "udp://") == 0)
	{
		currentOpenedMediaType = MEDIA_TYPE_SOCKET;
		mediaInPath = media;
		mediaInPath.erase(0, std::strlen("udp://"));		
		std::cout << "current media type : SOCKET " << std::endl; 

		// To do ...
	}
	else
	{
		currentOpenedMediaType = MEDIA_TYPE_NOTSUPPORT;
		std::cout << "current media type : Not Supported " << std::endl; 

		return RESULT_NOTOK;
	}

	std::cout << "mediaInPath : " << mediaInPath << std::endl;
	

	return RESULT_OK;
}

RESULT_T CSection::readMediaFromFile(SECTION_INFO_T* sectionInfo)
{
	int cnt;
	char buffer[188];
	bool transport_error_indicator;
	bool payload_unit_start_indicator;

	UINT16 pid;
	UINT8 scrambling_control;
	UINT8 adaptation_field_control;
	UINT8 continuity_counter;

	UINT8 moveSize;

	fin.read(buffer, 188);

	cnt = fin.gcount();
	if(cnt == 0)
	{
		std::cout << "No Data !! " << std::endl;
		return RESULT_NOTOK;
	}

	if(buffer[0] != 0x47)
	{
		std::cout << "Not Header Start " << std::endl;
		return RESULT_INVALID;
	}

	transport_error_indicator 		= (buffer[1] >> 7) & 0x01;
	payload_unit_start_indicator 	= (buffer[1] >> 6) & 0x01;
	
	pid = ((buffer[1] & 0x1f) << 8) | buffer[2];
	PRINT_SECTION_INFO(pid);

	scrambling_control 			= (buffer[3] >> 6) & 0x03;
	adaptation_field_control 	= (buffer[3] >> 4) & 0x03;
	continuity_counter			= buffer[3] & 0x0f;
	
	if(adaptation_field_control == 0x01)
	{
		std::memset(sectionInfo->section, 0x00, 188);
		std::memcpy(sectionInfo->section, buffer+4, 184);
	}
	else if(adaptation_field_control == 0x03) // adapation + Payload
	{
		moveSize = 0;

		moveSize = buffer[4] + 4 + 1;
		std::memset(sectionInfo->section, 0x00, 188);
		std::memcpy(sectionInfo->section, buffer + moveSize, 188 - moveSize);
	}
	else
	{
		sectionInfo->pid = 0;
		return RESULT_INVALID;
	}

	sectionInfo->pid = pid;

	return RESULT_OK;
}

RESULT_T CSection::readMedia(SECTION_INFO_T* sectionInfo)
{
	RESULT_T retval;
	switch(currentOpenedMediaType)
	{
		case MEDIA_TYPE_FILE:
			retval = readMediaFromFile(sectionInfo);
			return retval;
		break;
		default:
		break;
	}

	return RESULT_NOTOK;
}