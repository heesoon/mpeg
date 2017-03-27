#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <thread>
#include <functional>

#define MAX_FILTER_NUM 0x2000
#define PRINT_HEADER_MARK(c, l) {for(int i = 0; i < (l); i++) std::printf("%c", (c)); std::printf("\n");}
#define PRINT_HEADER_INFO(s, x) std::printf("%-30s : %10d [%#10x]\n", #s, x, x);

typedef struct _TSfilterInfo_t
{
	bool transport_error_indicator;
	bool payload_unit_start_indicator;
	bool transport_priority;

	unsigned short pid;								// 13 bit
	unsigned char transport_scrambling_control;		// 2 bit
	unsigned char adaptation_field_control;			// 2 bit
	unsigned char continuity_counter;				// 4 bit

	unsigned char pointer_field;					// 8 bit
	unsigned char payload_length;
	unsigned long long ts_packet_number;			// 64 bit
} TSfilterInfo_t;

class CTSfilter
{
private :
	std::thread filterTaskId[MAX_FILTER_NUM];
public  :
	CTSfilter();
	virtual ~CTSfilter();
};

CTSfilter::CTSfilter()
{
	std::cout << __FUNCTION__ << " called !!" << std::endl; 

	std::thread tr([](){
		unsigned long long cnt = 0;
		char buffer[188] = {0, };
		std::ifstream fin;
		
		fin.open("/home/hskim/share/fantasy_pi_pim.ts", std::ios_base::in | std::ios_base::binary);
		if(fin.is_open() == false)
		{
			std::cout << "file open error!!" << std::endl;
			return;
		}

		while(1)
		{
			TSfilterInfo_t tsInfo;

			fin.read(buffer, 188);

			if(fin.gcount() == 0)
				break;

			if(buffer[0] != 0x47)
			{
				std::cout << "Not Header Start " << std::endl;
			}

			tsInfo.ts_packet_number = cnt;
			tsInfo.transport_error_indicator 		= (buffer[1] >> 7) & 0x01;
			tsInfo.payload_unit_start_indicator 	= (buffer[1] >> 6) & 0x01;
			tsInfo.pid 								= ((buffer[1] & 0x1f) << 8) | buffer[2];
			tsInfo.transport_scrambling_control 	= (buffer[3] >> 6) & 0x03;
			tsInfo.adaptation_field_control 		= (buffer[3] >> 4) & 0x03;
			tsInfo.continuity_counter				= buffer[3] & 0x0f;

			PRINT_HEADER_MARK('-', 56)
			PRINT_HEADER_INFO(ts_packet_number				, tsInfo.ts_packet_number)
			PRINT_HEADER_INFO(transport_error_indicator		, tsInfo.transport_error_indicator)
			PRINT_HEADER_INFO(payload_unit_start_indicator	, tsInfo.payload_unit_start_indicator)
			PRINT_HEADER_INFO(pid 							, tsInfo.pid)
			PRINT_HEADER_INFO(transport_scrambling_control	, tsInfo.transport_scrambling_control)
			PRINT_HEADER_INFO(adaptation_field_control		, tsInfo.adaptation_field_control)
			PRINT_HEADER_INFO(continuity_counter			, tsInfo.continuity_counter)

			cnt++;
		}
	});

	std::cout << "end of read!!" << std::endl;

	tr.join();
}

CTSfilter::~CTSfilter()
{
	std::cout << __FUNCTION__ << " called !!" << std::endl;
}

int main(int argc, char *argv[])
{
	CTSfilter tsfilter;
	return 0;
}