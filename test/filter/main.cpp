#include "sifilter.hpp"
#include <fstream>
#include <iostream>

#define PRINT_HEADER_MARK(c, l) {for(int i = 0; i < (l); i++) std::printf("%c", (c)); std::printf("\n");}
#define PRINT_HEADER_INFO(s, x) std::printf("%-30s : %10d [%#10x]\n", #s, x, x);

struct TSfilterInfo_t
{
    bool transport_error_indicator;
    bool payload_unit_start_indicator;
    bool transport_priority;

    unsigned short pid;                             // 13 bit
    unsigned char transport_scrambling_control;     // 2 bit
    unsigned char adaptation_field_control;         // 2 bit
    unsigned char continuity_counter;               // 4 bit

    unsigned char pointer_field;                    // 8 bit
    unsigned char payload_length;
    unsigned long long ts_packet_number;            // 64 bit
};

int main(int argc, char *argv[])
{
#if 1    
    std::shared_ptr<CTSfilterManager> pSharedSiFiter = CTSfilterManager::getInstance();

    std::thread tr([&](){

        char buffer[188] = {0, };
        std::ifstream fin;

        fin.open("/home/hskim/share/fantasy_pi_pim.ts", std::ios_base::in | std::ios_base::binary);
        if(fin.is_open() == false)
        {
            std::cout << "file open error!!" << std::endl;
            return;
        }

        pSharedSiFiter->requestSectionFilter("pat_ts_payload", 0, [](char *buffer, size_t num){

            static unsigned long long cnt = 0;
            TSfilterInfo_t tsInfo   = {0, };
            tsInfo.ts_packet_number = cnt++;
            tsInfo.transport_error_indicator        = (buffer[1] >> 7) & 0x01;
            tsInfo.payload_unit_start_indicator     = (buffer[1] >> 6) & 0x01;
            tsInfo.pid                              = ((buffer[1] & 0x1f) << 8) | buffer[2];
            tsInfo.transport_scrambling_control     = (buffer[3] >> 6) & 0x03;
            tsInfo.adaptation_field_control         = (buffer[3] >> 4) & 0x03;
            tsInfo.continuity_counter               = buffer[3] & 0x0f;

            PRINT_HEADER_MARK('-', 56)
            PRINT_HEADER_INFO(ts_packet_number              , tsInfo.ts_packet_number)
            PRINT_HEADER_INFO(transport_error_indicator     , tsInfo.transport_error_indicator)
            PRINT_HEADER_INFO(payload_unit_start_indicator  , tsInfo.payload_unit_start_indicator)
            PRINT_HEADER_INFO(pid                           , tsInfo.pid)
            PRINT_HEADER_INFO(transport_scrambling_control  , tsInfo.transport_scrambling_control)
            PRINT_HEADER_INFO(adaptation_field_control      , tsInfo.adaptation_field_control)
            PRINT_HEADER_INFO(continuity_counter            , tsInfo.continuity_counter)            
        });

        while(1)
        {
            fin.read(buffer, 188);
            pSharedSiFiter->startReceiveBuffer(buffer, 188);

            if(fin.gcount() == 0)
            {
                std::cout << "gcount zero!! " << std::endl;
                break;
            }           
        }
    });

    tr.join();

    std::cout << "use_count : " << pSharedSiFiter.use_count() << std::endl;
    pSharedSiFiter->closeRequestSectionFilter(0);

    std::cout << "end of read!!" << std::endl;

#else

    CTSfilterManager *instance = CTSfilterManager::getInstance();
    
    std::thread tr([&](){

        char buffer[188] = {0, };
        std::ifstream fin;

        fin.open("/home/hskim/share/fantasy_pi_pim.ts", std::ios_base::in | std::ios_base::binary);
        if(fin.is_open() == false)
        {
            std::cout << "file open error!!" << std::endl;
            return;
        }

        instance->requestSectionFilter("pat_ts_payload", 0, [](char *buffer, size_t num){

            static unsigned long long cnt = 0;
            TSfilterInfo_t tsInfo   = {0, };
            tsInfo.ts_packet_number = cnt++;
            tsInfo.transport_error_indicator        = (buffer[1] >> 7) & 0x01;
            tsInfo.payload_unit_start_indicator     = (buffer[1] >> 6) & 0x01;
            tsInfo.pid                              = ((buffer[1] & 0x1f) << 8) | buffer[2];
            tsInfo.transport_scrambling_control     = (buffer[3] >> 6) & 0x03;
            tsInfo.adaptation_field_control         = (buffer[3] >> 4) & 0x03;
            tsInfo.continuity_counter               = buffer[3] & 0x0f;

            PRINT_HEADER_MARK('-', 56)
            PRINT_HEADER_INFO(ts_packet_number              , tsInfo.ts_packet_number)
            PRINT_HEADER_INFO(transport_error_indicator     , tsInfo.transport_error_indicator)
            PRINT_HEADER_INFO(payload_unit_start_indicator  , tsInfo.payload_unit_start_indicator)
            PRINT_HEADER_INFO(pid                           , tsInfo.pid)
            PRINT_HEADER_INFO(transport_scrambling_control  , tsInfo.transport_scrambling_control)
            PRINT_HEADER_INFO(adaptation_field_control      , tsInfo.adaptation_field_control)
            PRINT_HEADER_INFO(continuity_counter            , tsInfo.continuity_counter)            
        });

        while(1)
        {
            fin.read(buffer, 188);
            instance->startReceiveBuffer(buffer, 188);

            if(fin.gcount() == 0)
            {
                std::cout << "gcount zero!! " << std::endl;
                break;
            }           
        }
    });

    tr.join();

    instance->closeRequestSectionFilter(0);
    instance->closeTSFilterMgr();

    std::cout << "end of read!!" << std::endl;
#endif    
    return 0;
}
