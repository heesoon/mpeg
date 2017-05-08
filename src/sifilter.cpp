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
#include <algorithm>

enum class DebugLevel4SifilerManager
{
    DEBUG_LEVEL_NORMAL,     // No Debug Message from class
    DEBUG_LEVEL_ERROR,      // Print Error Message
    DEBUG_LEVEL_DEBUG,      // Print Debug Related Message
    DEBUG_LEVEL_TRACE,      // All Debug Message Trace including class member function
    DEBUG_LEVEL_MAX
};

static DebugLevel4SifilerManager gDebugLevel = DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE;

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

CTSfilterManager::CTSfilterManager()
{
    DEBUG_PRINT(DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE, "");
}

CTSfilterManager::~CTSfilterManager()
{
    for(auto iter : filterMgr)
    {
        delete iter;
    }

    filterMgr.clear();
    inst = nullptr;

    DEBUG_PRINT(DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE, "");
}

CTSfilterManager* CTSfilterManager::inst = nullptr;
CTSfilterManager* CTSfilterManager::getInstance(void)
{
    if(inst == nullptr)
    {
        inst = new CTSfilterManager();
    }

    DEBUG_PRINT(DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE, "");
    return inst;
}

//void CTSfilterManager::startReceiveBuffer(unsigned char *buffer, size_t num)
void CTSfilterManager::startReceiveBuffer(char *buffer, size_t num)
{
    unsigned short pid = ((buffer[1] & 0x1f) << 8) | buffer[2];

    for(auto iter = filterMgr.begin(); iter != filterMgr.end(); iter++)
    {
        if((*iter)->pid == pid)
        {
            (*iter)->pCallBack(buffer, num);
        }
    }
}

void CTSfilterManager::requestSectionFilter(std::string filterName, unsigned int pid, callBackType pCallBack)
{
    CTSfilter *filter;

    filter = new CTSfilter();

    filter->pid         = pid;
    filter->pCallBack   = pCallBack;
    filter->filterName  = filterName;

    filterMgr.push_back(filter);
}

void CTSfilterManager::closeRequestSectionFilter(unsigned int pid)
{
    for(auto iter = filterMgr.begin(); iter != filterMgr.end(); iter++)
    {
        if((*iter)->pid == pid)
        {
            filterMgr.erase(iter);
            break;
        }
    }
}

void CTSfilterManager::closeTSFilterMgr(void)
{
    if(inst == nullptr)
    {
        DEBUG_PRINT(DebugLevel4SifilerManager::DEBUG_LEVEL_TRACE, "Already Destroyed!!");
        return;
    }

    delete inst;
}