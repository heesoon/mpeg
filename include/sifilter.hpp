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

#ifndef __SIFILTER_HPP__
#define __SIFILTER_HPP__

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

class CTSfilter
{
public  :
    CTSfilter();
    virtual ~CTSfilter();
    //void requestSection(int pid);

protected :
    //std::thread filterTaskId[MAX_FILTER_NUM];
    //std::ifstream fin;
    //std::mutex mtx_lock;    
};

#endif