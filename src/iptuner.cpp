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
#include "iptuner.hpp"

enum class DebugLevel4IPTuner
{
    DEBUG_LEVEL_NORMAL,     // No Debug Message from class
    DEBUG_LEVEL_ERROR,      // Print Error Message
    DEBUG_LEVEL_DEBUG,      // Print Debug Related Message
    DEBUG_LEVEL_TRACE,      // All Debug Message Trace including class member function
    DEBUG_LEVEL_MAX
};

static DebugLevel4IPTuner gDebugLevel = DebugLevel4IPTuner::DEBUG_LEVEL_TRACE;

#define DEBUG_PRINT(debugPrint, debugMsg)\
{\
    if((debugPrint) <= gDebugLevel)\
    {\
        switch(debugPrint) \
        {\
            case DebugLevel4IPTuner::DEBUG_LEVEL_NORMAL:\
                std::printf("[%s,%d] %s \n", __FUNCTION__, __LINE__, debugMsg);\
            break;\
            case DebugLevel4IPTuner::DEBUG_LEVEL_ERROR:\
                std::printf("\033[0;31m Error in [%s,%d] %s \n", __FUNCTION__, __LINE__, debugMsg);\
            break;\
            case DebugLevel4IPTuner::DEBUG_LEVEL_DEBUG:\
                std::printf("\033[1;32m Debug in [%s,%d] %s \n", __FUNCTION__, __LINE__, debugMsg);\
            break;\
            case DebugLevel4IPTuner::DEBUG_LEVEL_TRACE:\
                std::printf("\033[1;33m Trace in [%s,%d] %s \n", __FUNCTION__, __LINE__, debugMsg);\
            break;\
            default :\
            break;\
        }\
        std::printf("\033[0m");\
    }\
}

CIPTuner::CIPTuner() : socket(io_service_), receiveTrId()
{
    DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");
    pCallBack = nullptr;
}

CIPTuner::~CIPTuner()
{
    DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");
    stopTune();
}

bool CIPTuner::startTune(std::string addr, unsigned short port)
{
	DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");

    if( (addr_.compare(addr) == 0) && (port_ == port) )
    {
        DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_ERROR, "same address and port");
        return false;
    }

    if(receiveTrId)
    {
        DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_ERROR, "Already Task Started");
        return false;
    }

	addr_ = addr;
	port_ = port;

    boost::asio::ip::address multicast_address  = boost::asio::ip::address::from_string(addr);
    boost::asio::ip::udp::endpoint endpoint 	= boost::asio::ip::udp::endpoint(multicast_address, port);
    
    socket.open(udp::v4());
    socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket.set_option(boost::asio::ip::multicast::enable_loopback(true));
    socket.bind(endpoint);

    // Join the multicast group.
    socket.set_option(boost::asio::ip::multicast::join_group(multicast_address));
  
    //socket.non_blocking(true);

    //boost::scoped_ptr<boost::asio::io_service::work> work_(new boost::asio::io_service::work(io_service_));
    io_service_.post(boost::bind(&CIPTuner::do_receive, this));

    receiveTrId.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));

    DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");
    return true;
}

void CIPTuner::handle_receiver(const boost::system::error_code& error, size_t bytes_transferred)
{
    DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");

    if (!error || error == boost::asio::error::message_size)
    {
#ifdef USE_BOOST_ARRAY      
        if(pCallBack != nullptr)
            pCallBack(recv_buffer);
#else
        if(pCallBack != nullptr)
            pCallBack(recv_buffer, bytes_transferred);      
#endif
        do_receive();
    }
}

void CIPTuner::do_receive(void)
{
    DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");

    boost::asio::ip::udp::endpoint sender_endpoint;

#ifdef USE_BOOST_ARRAY 
    socket.async_receive_from(boost::asio::buffer(recv_buffer), sender_endpoint,
                                boost::bind(&CIPTuner::handle_receiver, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
#else
    socket.async_receive_from(boost::asio::buffer(recv_buffer, BUFFER_SIZE), sender_endpoint,
                                boost::bind(&CIPTuner::handle_receiver, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)); 
#endif  
}

void CIPTuner::registerCallBackFunc(pCallBackFuncType pFunc)
{
    DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");
    pCallBack = pFunc;
}

bool CIPTuner::stopTune(void)
{
    DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");

    boost::asio::ip::address multicast_address = boost::asio::ip::address::from_string(addr_);

    if(!receiveTrId)
    {
        DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_ERROR, "Task is Null");
        return false;
    }

    socket.set_option(boost::asio::ip::multicast::leave_group(multicast_address));
    socket.close();

    io_service_.stop();
    receiveTrId->join();
    io_service_.reset();
    receiveTrId.reset();

    DEBUG_PRINT(DebugLevel4IPTuner::DEBUG_LEVEL_TRACE, "");
    return true;
}