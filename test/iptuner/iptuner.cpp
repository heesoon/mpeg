#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <functional>
#include <mutex>

using boost::asio::ip::udp;

const static int BUFFER_SIZE = 188*7;

class CIPTuner
{
public  :
    CIPTuner();
    virtual ~CIPTuner();
    bool startTune(std::string addr, unsigned short port);
    bool stopTune(void);
    void do_receive(void);
    void handle_receiver(const boost::system::error_code& error, size_t bytes_transferred);
    using pCallBackFuncType = std::function<void(boost::array<unsigned char, BUFFER_SIZE>)>;
    pCallBackFuncType pCallBack;
    void registerCallBackFunc(pCallBackFuncType pFunc);
private :
    std::string addr;
    std::mutex mtx;
    unsigned short port;
    boost::asio::io_service io;
    boost::asio::ip::udp::socket socket;
    boost::array<unsigned char, BUFFER_SIZE> recv_buffer;
};

CIPTuner::CIPTuner() : socket(io)
{
    std::cout << __FUNCTION__ << std::endl;
    pCallBack = nullptr;
}

CIPTuner::~CIPTuner()
{
    std::cout << __FUNCTION__ << std::endl;
}

bool CIPTuner::startTune(std::string addr, unsigned short port)
{
    std::cout << __FUNCTION__ << std::endl;
    
    boost::asio::ip::address multicast_address  = boost::asio::ip::address::from_string(addr);
    boost::asio::ip::udp::endpoint endpoint 	= boost::asio::ip::udp::endpoint(multicast_address, port);
    
    socket.open(udp::v4());
    socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket.bind(endpoint);

    // Join the multicast group.
    socket.set_option(boost::asio::ip::multicast::join_group(multicast_address));
  
  	do_receive();

    io.run();
    
    return true;
}

bool CIPTuner::stopTune(void)
{
	std::cout << __FUNCTION__ << std::endl;

    io.stop();
    socket.close();

    return true;
}

void CIPTuner::do_receive(void)
{
	std::cout << __FUNCTION__ << std::endl;

	boost::asio::ip::udp::endpoint sender_endpoint;

	socket.async_receive_from(boost::asio::buffer(recv_buffer), sender_endpoint,
		                        boost::bind(&CIPTuner::handle_receiver, this,
		                        boost::asio::placeholders::error,
		                        boost::asio::placeholders::bytes_transferred));	
}

void CIPTuner::handle_receiver(const boost::system::error_code& error, size_t bytes_transferred)
{
	std::cout << __FUNCTION__ << std::endl;

	//std::cout << "Received: '" << std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred) << "'\n";
	if (!error || error == boost::asio::error::message_size)
	{
		if(pCallBack != nullptr)
			pCallBack(recv_buffer);

        do_receive();
	}
}

void CIPTuner::registerCallBackFunc(pCallBackFuncType pFunc)
{
	pCallBack = pFunc;
}

int main()
{
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

	int cnt = 0;

	try
	{
	    CIPTuner iptuner;
	    
	    std::string addr = "234.1.1.1";
	    unsigned short port = 5000;
	    
	    iptuner.registerCallBackFunc([&](boost::array<unsigned char, BUFFER_SIZE> _buffer){

	    	for(int i = 0; i < 7; i++)
	    	{

	    		unsigned char *buffer = &_buffer[188*i];
			    TSfilterInfo_t tsInfo;

			    if(buffer[0] != 0x47)
			    {
			        std::cout << "Not Header Start " << std::endl;
			    }

			    tsInfo.ts_packet_number = ++cnt;
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
			}
	    });

	    iptuner.startTune(addr, port);
	    iptuner.stopTune();	
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
    
    return 0;
}