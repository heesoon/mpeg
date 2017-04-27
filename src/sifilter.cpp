#if 0

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <thread>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>

#define MAX_FILTER_NUM 0x2000
#define PRINT_HEADER_MARK(c, l) {for(int i = 0; i < (l); i++) std::printf("%c", (c)); std::printf("\n");}
#define PRINT_HEADER_INFO(s, x) std::printf("%-30s : %10d [%#10x]\n", #s, x, x);

typedef struct _TSfilterInfo_t
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
} TSfilterInfo_t;

class CTSfilter
{
//private :
public :
    std::thread filterTaskId[MAX_FILTER_NUM];
    std::ifstream fin;
    std::mutex mtx_lock;
public  :
    CTSfilter();
    virtual ~CTSfilter();
    void requestSection(int pid);
};

CTSfilter::CTSfilter()
{
    std::cout << __FUNCTION__ << " called !!" << std::endl;
    fin.open("/home/hskim/share/fantasy_pi_pim.ts", std::ios_base::in | std::ios_base::binary);
    if(fin.is_open() == false)
    {
        std::cout << "file open error!!" << std::endl;
        return;
    }   
#if 0
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
            cnt++;
        }
    });
    std::cout << "end of read!!" << std::endl;
    tr.join();
#endif      
}

CTSfilter::~CTSfilter()
{
    std::cout << __FUNCTION__ << " called !!" << std::endl;
}

void CTSfilter::requestSection(int pid)
{
    filterTaskId[pid] = std::thread([&](int pid)
    {
        printf("pid = %d \n", pid);

        unsigned long long cnt = 0;
        char buffer[188] = {0, };

        while(1)
        {
            TSfilterInfo_t tsInfo = {0, };

            std::lock_guard<std::mutex> guard(mtx_lock);
            fin.read(buffer, 188);

            if(fin.gcount() == 0)
            {
                std::cout << "gcount zero!! " << std::endl;
                break;
            }

            //if(buffer[0] != 0x47)
            //{
            //  std::cout << "Not Header Start " << std::endl;
            //}

            tsInfo.ts_packet_number = cnt;
            tsInfo.transport_error_indicator        = (buffer[1] >> 7) & 0x01;
            tsInfo.payload_unit_start_indicator     = (buffer[1] >> 6) & 0x01;
            tsInfo.pid                              = ((buffer[1] & 0x1f) << 8) | buffer[2];
            tsInfo.transport_scrambling_control     = (buffer[3] >> 6) & 0x03;
            tsInfo.adaptation_field_control         = (buffer[3] >> 4) & 0x03;
            tsInfo.continuity_counter               = buffer[3] & 0x0f;

            if(pid == tsInfo.pid)
                printf("pid = %x tsInfo.pid = %x \n", pid, tsInfo.pid);
#if 0
            PRINT_HEADER_MARK('-', 56)
            PRINT_HEADER_INFO(ts_packet_number              , tsInfo.ts_packet_number)
            PRINT_HEADER_INFO(transport_error_indicator     , tsInfo.transport_error_indicator)
            PRINT_HEADER_INFO(payload_unit_start_indicator  , tsInfo.payload_unit_start_indicator)
            PRINT_HEADER_INFO(pid                           , tsInfo.pid)
            PRINT_HEADER_INFO(transport_scrambling_control  , tsInfo.transport_scrambling_control)
            PRINT_HEADER_INFO(adaptation_field_control      , tsInfo.adaptation_field_control)
            PRINT_HEADER_INFO(continuity_counter            , tsInfo.continuity_counter)
#endif
            cnt++;
        }
    }, pid);

    std::cout << "end of read!!" << std::endl;

    //tr.join();
}

int main(int argc, char *argv[])
{   
    CTSfilter tsfilter;
    tsfilter.requestSection(0);
    tsfilter.requestSection(1);
    tsfilter.requestSection(48);
    tsfilter.requestSection(2);
    //for(int i = 0; i < MAX_FILTER_NUM; i++)
    {
        tsfilter.filterTaskId[0].join();
        tsfilter.filterTaskId[1].join();
        //tsfilter.filterTaskId[1].join();
        tsfilter.filterTaskId[48].join();
        tsfilter.filterTaskId[2].join();
    }

    return 0;
}
#endif

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

#include "sifilter.hpp"
#include <iostream>

enum class DebugLevel4SifilerManager
{
    DEBUG_LEVEL_NORMAL,     // No Debug Message from class
    DEBUG_LEVEL_ERROR,      // Print Error Message
    DEBUG_LEVEL_DEBUG,      // Print Debug Related Message
    DEBUG_LEVEL_TRACE,      // All Debug Message Trace including class member function
    DEBUG_LEVEL_MAX
};

DebugLevel4SifilerManager gDebugLevel = DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE;

#define DEBUG_PRINT(debugPrint, debugMsg)\
{\
    if((debugPrint) <= gDebugLevel)\
    {\
        switch(debugPrint) \
        {\
            case DebugLevel4SifilerManager::DEBUG_LEVEL_NORMAL:\
                std::printf("[%s,%d] %s \n", __FUNCTION__, __LINE__, debugMsg);\
            break;\
            case DebugLevel4SifilerManager::DEBUG_LEVEL_ERROR:\
                std::printf("\033[0;31m Error in [%s,%d] %s \n", __FUNCTION__, __LINE__, debugMsg);\
            break;\
            case DebugLevel4SifilerManager::DEBUG_LEVEL_DEBUG:\
                std::printf("\033[1;32m Debug in [%s,%d] %s \n", __FUNCTION__, __LINE__, debugMsg);\
            break;\
            case DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE:\
                std::printf("\033[1;33m Trace in [%s,%d] %s \n", __FUNCTION__, __LINE__, debugMsg);\
            break;\
            default :\
            break;\
        }\
        std::printf("\033[0m");\
    }\
}

CTSfilter::CTSfilter()
{
    DEBUG_PRINT(DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE, "");
}

CTSfilter::~CTSfilter()
{
    DEBUG_PRINT(DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE, "");
}