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
	file : pat.cpp
	parsing program associate table information

	version : 1.0
	data    : 2016.06.01
	author  : HeeSoon Kim(chipmaker0304@gmail.com)
-----------------------------------------------------------------------*/

#include "pat.h"

PAT::PAT()
{
	numProgs = 0;
	std::cout << "pat generator called !! " << std::endl;
}

PAT::~PAT()
{
	patList.clear();
	numProgs = 0;

	std::cout << "pat destroy called !! " << std::endl;
}

bool PAT::isPmtPid(UINT16 pmtPid)
{
	for(auto patlist : patList)
	{
		if(patlist.pmtPid == pmtPid)
			return true;
	}

	return false;
}

bool PAT::isExistPat(UINT8 vserion_number, UINT8 section_number)
{
	for(auto program : patList)
	{
		if( (program.version == vserion_number) && (program.section_number == section_number) )
			return true;
		else
			return false;
	}
}

UINT32 PAT::getPATProgNum(void)
{
	return numProgs; 
}

std::forward_list<PROGRAM_T>& PAT::getPatList(void)
{
	return patList;
}

void PAT::printPATList(void)
{
	int i = 1;

	// sort by porgram number
	patList.sort([](PROGRAM_T& prog1, PROGRAM_T& prog2) 
					-> bool { return prog1.program_number < prog2.program_number; });

	for(auto program : patList)
	{
		std::printf("%3d] PAT version = %2d Section = %2d program_number = %3d [PID %5d (%#5x)]\n" \
		, i, program.version, program.section_number, program.program_number, program.pmtPid, program.pmtPid);

		i++;
	}
}

RESULT_T PAT::parsingPATSection(UINT8 *pData)
{
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


	if( isExistPat(version_number, section_number) == true )
	{
		return RESULT_NOTOK;
	}

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

			patList.push_front(program);
			numProgs++;
		}
		pSection += 4;
		section_length -= 4;
	}

	return RESULT_OK;
}