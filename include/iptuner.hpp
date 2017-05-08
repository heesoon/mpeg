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

#ifndef __IPTUNER_HPP__
#define __IPTUNER_HPP__

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <functional>
#include <boost/smart_ptr.hpp>

using boost::asio::ip::udp;

class CIPTuner
{
public  :
#ifdef USE_BOOST_ARRAY
	const static int BUFFER_SIZE = 188*7;
	using pCallBackFuncType = std::function<void(boost::array<unsigned char, BUFFER_SIZE>)>;
#else
	using pCallBackFuncType = std::function<void(char*, size_t)>;
#endif

public  :
    CIPTuner();
    virtual ~CIPTuner();
    virtual bool startTune(std::string addr, unsigned short port);
    virtual bool stopTune(void);
    virtual void do_receive(void);    
    virtual void handle_receiver(const boost::system::error_code& error, size_t bytes_transferred);
    virtual void registerCallBackFunc(pCallBackFuncType pFunc);
private :
    std::string addr_;
    unsigned short port_;
    boost::asio::io_service io_service_;
    boost::asio::ip::udp::socket socket;    
    boost::scoped_ptr<boost::thread> receiveTrId; 	
    pCallBackFuncType pCallBack;
#ifdef USE_BOOST_ARRAY    
    boost::array<unsigned char, BUFFER_SIZE> recv_buffer;
#else    
    enum { BUFFER_SIZE = 188*7 };
    char recv_buffer[BUFFER_SIZE];
#endif     
};
#endif