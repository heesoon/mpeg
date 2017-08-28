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
#include <string>
#include "pat.h"

//#define PRINT_PAT_HEADER_INFO(x) std::printf("%30s = %10d [%#10x]\n", #x, x, x);
#define PRINT_PAT_HEADER_INFO(x)
#define PAT_HEADER_SIZE 	8
#define GET_2BYTE(x) 		(((*(x)) << 8)|((*(x+1))))
#define GET_PID(x) 			((GET_2BYTE(x))&0x1FFF)

CPAT::CPAT() : pmtPid(0) {
	pMsgQ = CMsgQManager<FilterMessage>::getInstance().getMsgQ(PSI_INFO_MSGS);
	status = FilterStatus::FILTER_INITED;
}

CPAT::CPAT(const SectionFilterType& t) : pmtPid(0) {
	pMsgQ = CMsgQManager<FilterMessage>::getInstance().getMsgQ(PSI_INFO_MSGS);
	type = t;
}

CPAT::CPAT(const UINT16& pmtPid, const SectionFilterType& t) : pmtPid(pmtPid) {
	pMsgQ = CMsgQManager<FilterMessage>::getInstance().getMsgQ(PSI_INFO_MSGS);
	type = t;	
}

std::vector<PROGRAM_T>& CPAT::getPATInfo() {
	return v;
}

void CPAT::savePatToJson() {
	std::ofstream os("result/pat_result.txt");
	//std::ofstream os("/var/www/html/pat_result.txt");

	json_spirit::Array arr;

	// sort by porgram number
	//v.sort([](PROGRAM_T& prog1, PROGRAM_T& prog2) 
	//				-> bool { return prog1.program_number < prog2.program_number; });

	for(auto program : v)
	{
		json_spirit::Object obj;

		obj.push_back(json_spirit::Pair("version", program.version));
		obj.push_back(json_spirit::Pair("section_number", program.section_number));
		obj.push_back(json_spirit::Pair("program_number", program.program_number));
		obj.push_back(json_spirit::Pair("pmtPid", program.pmtPid));

		arr.push_back(obj);
	}

	json_spirit::write_formatted(arr, os);

	os.close();	
}

void CPAT::notify(const FilterStatus& stat) {
	
}

UINT32 CPAT::getNumberOfProgram() {
	return numProgs;
}

bool CPAT::isExistSection(UINT8 version, UINT8 section_number) {
	if(this->version == version) {
		if( b.test(section_number) == true )  {
			return true;
		}
		else {
			b.set(section_number);
			return false;
		}
	}
	else {
		// version up case check
		pMsgQ->sendMsg(FilterMessage::PAT_VERSION_UP);
	}

	return false;
}

void CPAT::parsing(UINT8 *pData) {
	bool section_syntax_indicator;
	bool current_next_indicator;

	UINT8 *pSection;
	UINT8 point_field;
	UINT8 table_id;
	UINT8 version_number;
	UINT8 section_number;
	UINT8 last_section_number;	

	UINT16 section_length;
	UINT16 transport_stream_id;

	PROGRAM_T program;

	point_field = pData[0];
	PRINT_PAT_HEADER_INFO(point_field);

	pSection = pData + point_field + 1;

	table_id = pSection[0];
	PRINT_PAT_HEADER_INFO(table_id);

	section_syntax_indicator = (pSection[1] >> 7) & 0x01;
	PRINT_PAT_HEADER_INFO(section_syntax_indicator);

	section_length = ((pSection[1] & 0x0F) << 8) | pSection[2];
	PRINT_PAT_HEADER_INFO(section_length);

	transport_stream_id = (pSection[3] << 8) | pSection[4];
	PRINT_PAT_HEADER_INFO(transport_stream_id);	

	version_number = (pSection[5] >> 1) & 0x1F;
	PRINT_PAT_HEADER_INFO(version_number);

	current_next_indicator = pSection[5] & 0x01;
	PRINT_PAT_HEADER_INFO(current_next_indicator);

	section_number = pSection[6];
	PRINT_PAT_HEADER_INFO(section_number);

	last_section_number = pSection[7];
	PRINT_PAT_HEADER_INFO(last_section_number);

    // filter status update
	if(status == FilterStatus::FILTER_INITED) {
		this->version 				= version_number;
		this->last_section_number	= last_section_number;
		status = FilterStatus::FILTER_STARTED;
	}

	if(isExistSection(version_number, section_number) == true) {
		//std::cout << "it is duplication section ... " << std::endl;
		return;
	}

	// filter version up
	if(status == FilterStatus::FILTER_VERSION_UP) {
		status = FilterStatus::FILTER_INITED;
		b.reset();
		v.clear();
	}

	// filter status update
	if(status == FilterStatus::FILTER_STARTED) {
		status = FilterStatus::FILTER_PARSING;
	}

	this->section_count++;

	pSection = pData + point_field + PAT_HEADER_SIZE + 1;
	section_length -= PAT_HEADER_SIZE + 1;

	while(section_length >= 4)
	{
		program.program_number = GET_2BYTE(pSection);
		if(program.program_number != 0) // Ignore NIT
		{
			program.version 		= version_number;
			program.section_number 	= section_number;
			program.pmtPid 			= GET_PID(pSection+2);

			v.push_back(program);
			numProgs++;
		}
		pSection += 4;
		section_length -= 4;
	}

	// filter status update
	if(status == FilterStatus::FILTER_PARSING) {
		if(this->section_count == this->last_section_number+1) {
			status = FilterStatus::FILTER_PARSING_DONE;
			pMsgQ->sendMsg(FilterMessage::PAT_PARSING_DONE);
		}
	}
}

CPAT::~CPAT() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
	v.clear();
}
