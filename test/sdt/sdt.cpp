
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
	file : sdt.cpp
	parsing service description table

	version : 1.0
	data    : 2016.06.01
	author  : HeeSoon Kim(chipmaker0304@gmail.com)
-----------------------------------------------------------------------*/

/*
service_description_section(){
	table_id 						8 uimsbf
	section_syntax_indicator 		1 bslbf
	reserved_future_use 			1 bslbf
	reserved 						2 bslbf
	section_length 					12 uimsbf
	transport_stream_id 			16 uimsbf
	reserved 						2 bslbf
	version_number 					5 uimsbf
	current_next_indicator 			1 bslbf
	section_number 					8 uimsbf
	last_section_number 			8 uimsbf
	original_network_id 			16 uimsbf
	reserved_future_use 			8 bslbf
	
	for (i=0;i<N;i++){
		service_id 					16 uimsbf
		reserved_future_use 		6 bslbf
		EIT_schedule_flag 			1 bslbf
		EIT_present_following_flag 	1 bslbf
		running_status 				3 uimsbf
		free_CA_mode 				1 bslbf
		descriptors_loop_length 	12 uimsbf

		for (j=0;j<N;j++){
			descriptor()
		}
	}
	CRC_32 32 rpchof
}
*/

#include "sdt.h"

SDT::SDT()
{
	NumOfService = 0;
	std::cout << __FUNCTION__ << std::endl;
}

SDT::~SDT()
{
	NumOfService = 0;
	sdt_info_list.clear();

	std::cout << __FUNCTION__ << std::endl;
}

bool SDT::isSDTSecton(UINT16 pid)
{
	if(pid == SDT_ACTUAL_PID)
		return true;
	else
		return false;
}

bool SDT::isExistSdt(UINT8 version_number, UINT8 section_number, UINT16 program_number)
{
	for(auto list : sdt_info_list)
	{
		if(list.version == version_number && list.section_number == section_number && list.program_number == program_number)
		{
			return true;
		}
	}

	return false;
}

void SDT::printSDTList(void)
{
	int i = 0;

	sdt_info_list.sort([](SDT_INFO_T& prog1, SDT_INFO_T& prog2) 
					-> bool { return prog1.program_number < prog2.program_number; });

	for(auto list : sdt_info_list)
	{
		std::printf("%3d] SDT version = %2d, Section = %2d, service_id = %3d \n" \
		, i, list.version, list.section_number, list.program_number);

		std::printf("%3c %30s : %5d  \n", '-', "EIT_schedule_flag", list.EIT_schedule_flag);
		std::printf("%3c %30s : %5d  \n", '-', "EIT_present_following_flag", list.EIT_present_following_flag);
		std::printf("%3c %30s : %#5x \n", '-', "running_status", list.running_status);
		std::printf("%3c %30s : %5d  \n", '-', "free_CA_mode", list.free_CA_mode);
		std::printf("%3c %30s : %s   \n", '-', "service Provide Name", list.servicProvideeName.data());
		std::printf("%3c %30s : %s   \n", '-', "service Name", list.serviceName.data());

		i++;
	}
}

RESULT_T SDT::parsingServiceDescriptor(SDT_INFO_T* sdtInfo, UINT8 *pData)
{
	UINT8 descriptor_tag;
	UINT8 descriptor_length;
	UINT8 service_type;
	UINT8 service_provider_name_length;
	UINT8 service_name_length;

	descriptor_tag 					= pData[0];
	descriptor_length 				= pData[1];
	service_type 					= pData[2];
	service_provider_name_length 	= pData[3];

	sdtInfo->servicProvideeName.assign((char*)pData+4, service_provider_name_length);

	pData = pData + 3 + service_provider_name_length + 1;

	service_name_length = pData[0];
	sdtInfo->serviceName.assign((char*)pData+1, service_name_length + 1);
}

RESULT_T SDT::parsingSDTSection(UINT8 *pData)
{
	bool EIT_schedule_flag;
	bool EIT_present_following_flag;
	bool free_CA_mode;

	UINT8 *pSection;
	UINT8 point_field;
	UINT8 table_id;
	UINT8 version_number;
	UINT8 section_number;
	UINT8 last_section_number;
	UINT8 running_status;

	UINT16 section_length;
	UINT16 transport_stream_id;
	UINT16 original_network_id;
	UINT16 service_id;
	UINT16 descriptors_loop_length;	

	point_field = pData[0];
	PRINT_SDT_INFO(point_field);

	pSection = pData + point_field + 1;

	table_id = pSection[0];
	PRINT_SDT_INFO(table_id);

	if(table_id != SDT_ACTUAL_TABLE_ID)
	{
		return RESULT_NOTOK;
	}

	section_length = ((pSection[1] & 0x0f) << 8) | pSection[2];
	PRINT_SDT_INFO(section_length);

	transport_stream_id = (pSection[3] << 8) | pSection[4];
	PRINT_SDT_INFO(transport_stream_id);

	version_number = (pSection[5] >> 1) & 0x1f;
	PRINT_SDT_INFO(version_number);

	section_number = pSection[6];
	PRINT_SDT_INFO(section_number);

	last_section_number = pSection[7];
	PRINT_SDT_INFO(last_section_number);

	original_network_id = (pSection[8] << 8) | pSection[9];
	PRINT_SDT_INFO(original_network_id);

	pSection = pData + point_field + SDT_HEADER_SIZE + 1;
	section_length -= SDT_HEADER_SIZE + 1;	

	while(section_length > 0)
	{
		SDT_INFO_T sdtInfo = {0, };

		service_id = (pSection[0] << 8) | pSection[1];
		PRINT_SDT_INFO(service_id);

		if(isExistSdt(version_number, section_number, service_id) == true)
			return RESULT_OK;

		EIT_schedule_flag = (pSection[2] >> 1) & 0x01;
		PRINT_SDT_INFO(EIT_schedule_flag);

		EIT_present_following_flag = pSection[2] & 0x01;		
		PRINT_SDT_INFO(EIT_present_following_flag);

		running_status = (pSection[3] >> 5) & 0x07;
		PRINT_SDT_INFO(running_status);

		free_CA_mode = (pSection[3] >> 4) & 0x01;
		PRINT_SDT_INFO(free_CA_mode);

		descriptors_loop_length = ((pSection[3] & 0x0f) << 8) | pSection[4];
		PRINT_SDT_INFO(descriptors_loop_length);

		sdtInfo.version 					= version_number;
		sdtInfo.section_number 				= section_number;
		sdtInfo.program_number 				= service_id;
		sdtInfo.EIT_schedule_flag 			= EIT_schedule_flag;
		sdtInfo.EIT_present_following_flag 	= EIT_present_following_flag;
		sdtInfo.running_status				= running_status;
		sdtInfo.free_CA_mode				= free_CA_mode;

		parsingServiceDescriptor(&sdtInfo, pSection+5);
		//sdtInfo.serviceName.copy(0, pSection[])

		sdt_info_list.push_front(sdtInfo);

		pSection 		+= (5 + descriptors_loop_length);
		section_length 	-= (5 + descriptors_loop_length);
	}

	return RESULT_OK;
}
