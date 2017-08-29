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
#include "pmt.h"

CPMT::CPMT(const UINT16& pid, const SectionFilterType& t) : pid(pid) {
	pMsgQ = CMsgQManager<FilterMessage>::getInstance().getMsgQ(PSI_INFO_MSGS);
	status = FilterStatus::FILTER_INITED;
	type = t;
	b.reset();
}

UINT32 CPMT::getPMTProgNum(void)
{
	return numProgs;
}

void CPMT::addNewPMTtoList(PMT_INFO_T newPmt)
{
	pmtList.push_front(newPmt);
}

void CPMT::addNewEStoPMT(PMT_INFO_T newPmt, ES_INFO_T newEs)
{
	//pmtList.push_front(es_list);
	for(auto &pmt_list : pmtList)
	{
		if(pmt_list.program_number == newPmt.program_number)
		{
			pmt_list.es_info.push_front(newEs);
		}
	}
}

void CPMT::printPMTList(void)
{
	int i = 1;
	int j = 1;

	// sort by porgram number
	//pmtList.sort([](PMT_INFO_T& prog1, PMT_INFO_T& prog2) 
	//				-> bool { return prog1.program_number < prog2.program_number; });

	for(auto list : pmtList)
	{
		std::printf("%3d] PMT version = %2d Section = %2d program_number = %3d [PCR PID %5d (%#5x)]\n" \
		, i, list.version, list.section_number, list.program_number, list.pcr_pid, list.pcr_pid);

		for(auto es_list : list.es_info)
		{
			std::printf("  -> [%3d] stream_type = %#5x [ES PID %5d (%#5x)]\n" \
			, j, es_list.stream_type, es_list.es_pid, es_list.es_pid);

			j++;
		}

		i++;
		j = 1;
	}
}

void CPMT::savePmtToJson(void)
{
	// sort by porgram number
	//pmtList.sort([](PMT_INFO_T& prog1, PMT_INFO_T& prog2) 
	//				-> bool { return prog1.program_number < prog2.program_number; });

	std::ofstream os("result/pmt_json.txt", std::ofstream::app);

	json_spirit::Array pmtArr;

	for(auto list : pmtList)
	{
		json_spirit::Object pmtObj;

		pmtObj.push_back(json_spirit::Pair("version", list.version));
		pmtObj.push_back(json_spirit::Pair("section_number", list.section_number));
		pmtObj.push_back(json_spirit::Pair("program_number", list.program_number));
		pmtObj.push_back(json_spirit::Pair("pcr_pid", list.pcr_pid));

		json_spirit::Array esArr;
		for(auto es_list : list.es_info)
		{
			json_spirit::Object esObj;

			esObj.push_back(json_spirit::Pair("stream_type", es_list.stream_type));
			esObj.push_back(json_spirit::Pair("es_pid", es_list.es_pid));

			esArr.push_back(esObj);
		}		

		pmtObj.push_back(json_spirit::Pair("esInfo", esArr));

		pmtArr.push_back(pmtObj);
	}

	json_spirit::write_formatted(pmtArr, os);

	os.close();
}

bool CPMT::isExistSection(UINT8 version, UINT8 section_number, UINT16 program_number) {
	if(this->program_number != program_number) {
		//std::cout << "program number mismatch ..." << std::endl;
	}

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
		pMsgQ->sendMsg(FilterMessage::PMT_VERSION_UP);
	}

	return false;
}

void CPMT::notify(const FilterStatus& stat) {

}

void CPMT::parsing(UINT8 *pData, UINT16 pid) {

	if(this->pid != pid) {
		std::cout << "pid mismatched ... " << std::endl;
		return;
	}

	UINT8 *pSection;
	UINT8 point_field;

	UINT8 table_id;
	UINT16 section_length;
	UINT16 program_number;
	UINT8 version_number;
	bool current_next_indicator;
	UINT8 section_number;
	UINT8 last_section_number;
	UINT16 PCR_PID;
	UINT16 program_info_length;

	UINT8 stream_type;
	UINT16 elementary_PID;
	UINT16 ES_info_length;
	UINT32 descriptor_length;

	PMT_INFO_T pmt_info;
	ES_INFO_T es_info;

	point_field = pData[0];
	//PRINT_PMT_INFO(point_field);

	pSection = pData + point_field + 1;

	table_id = pSection[0];
	//PRINT_PMT_INFO(table_id);

	section_length = ((pSection[1] & 0x0f) << 8) | pSection[2];
	PRINT_PMT_INFO(section_length);

	program_number = (pSection[3] << 8) | pSection[4];
	//PRINT_PMT_INFO(program_number);

	version_number = (pSection[5] >> 0x01) & 0x1f;
	//PRINT_PMT_INFO(version_number);

	current_next_indicator = pSection[5] & 0x01;
	//PRINT_PMT_INFO(current_next_indicator);

	section_number = pSection[6];
	//PRINT_PMT_INFO(section_number);

	last_section_number = pSection[7];
	//PRINT_PMT_INFO(last_section_number);

	PCR_PID = ((pSection[8] & 0x1f) << 8) | pSection[9];
	PRINT_PMT_INFO(PCR_PID);

	program_info_length = ((pSection[10] & 0x0f) << 8) | pSection[11];
	PRINT_PMT_INFO(program_info_length);

	pSection = pData + point_field + PMT_HEADER_SIZE + program_info_length + 1;
	section_length -= PMT_HEADER_SIZE + 1 + program_info_length;

    // filter status update
	if(status == FilterStatus::FILTER_INITED) {
		this->version 				= version_number;
		this->last_section_number	= last_section_number;
		this->program_number		= program_number;
		status = FilterStatus::FILTER_STARTED;
	}

	if(isExistSection(version_number, section_number, program_number) == true) {
		//std::cout << "it is duplication section ... " << std::endl;
		return;
	}

	// filter version up
	if(status == FilterStatus::FILTER_VERSION_UP) {
		status = FilterStatus::FILTER_INITED;
		b.reset();
		for(auto& fl : pmtList) {
			fl.es_info.clear();
		}
		pmtList.clear();
	}

	// filter status update
	if(status == FilterStatus::FILTER_STARTED) {
		status = FilterStatus::FILTER_PARSING;
	}

	this->section_count++;

	// add new PMT to list
	pmt_info.version 			= version_number;
	pmt_info.section_number 	= section_number;
	pmt_info.program_number		= program_number;
	pmt_info.pcr_pid 			= PCR_PID;

	numProgs++;

	addNewPMTtoList(pmt_info);

	while(section_length > 0)
	{
		stream_type = pSection[0];
		PRINT_PMT_INFO(stream_type);

		elementary_PID = ((pSection[1] & 0x1f) << 8) | pSection[2];
		PRINT_PMT_INFO(elementary_PID);

		ES_info_length = ((pSection[3] & 0x0f) << 8) | pSection[4];
		PRINT_PMT_INFO(ES_info_length);

		es_info.stream_type = stream_type;
		es_info.es_pid		= elementary_PID;
		
		addNewEStoPMT(pmt_info, es_info);

		if(ES_info_length+5 > section_length)
			break;

		pSection 		+= 5;
		section_length 	-= (ES_info_length + 5);

		while(ES_info_length > 0)
		{
			// Parsing Descriptor
			descriptor_length = pSection[1] + 2;
			if(descriptor_length > ES_info_length)
			{
				pSection += ES_info_length;
				ES_info_length = 0;
				break;
			}

			ES_info_length 	-= descriptor_length;
			pSection		+= descriptor_length;
		}
	}

	// filter status update
	if(status == FilterStatus::FILTER_PARSING) {
		if(this->section_count == this->last_section_number+1) {
			status = FilterStatus::FILTER_PARSING_DONE;
			pMsgQ->sendMsg(FilterMessage::PMT_PARSING_DONE);
			printPMTList();
			savePmtToJson();
		}
	}
}

CPMT::~CPMT() {
	std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
}