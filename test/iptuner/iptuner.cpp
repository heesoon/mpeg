#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "boost/bind.hpp"

using boost::asio::ip::udp;

class CIPTuner
{
public  :
    CIPTuner();
    virtual ~CIPTuner();
    bool startTune(std::string addr, unsigned short port);
    bool stopTune();

  void handle_receive_from(const boost::system::error_code& error,
      size_t bytes_recvd)
  {
    if (!error)
    {
      std::cout.write(data_, bytes_recvd);
      std::cout << std::endl;

      socket.async_receive_from(
          boost::asio::buffer(data_, max_length), sender_endpoint_,
          boost::bind(&CIPTuner::handle_receive_from, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
  }

private :
    std::string addr;
    unsigned short port;
    boost::asio::io_service io;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint sender_endpoint_;

   enum { max_length = 1024 };
   char data_[max_length];   
};

CIPTuner::CIPTuner() : socket(io)
{
    std::cout << __FUNCTION__ << std::endl;
    //socket(io);
}

CIPTuner::~CIPTuner()
{
    std::cout << __FUNCTION__ << std::endl;
}

bool CIPTuner::startTune(std::string addr, unsigned short port)
{
    std::cout << __FUNCTION__ << std::endl;
    
    udp::endpoint endpoint = boost::asio::ip::udp::endpoint(
        boost::asio::ip::address::from_string(addr), port);
    
    socket.open(udp::v4());
    socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket.bind(endpoint);
    
    // Join the multicast group.
    boost::asio::ip::address multicast_address = boost::asio::ip::address::from_string(addr);
    socket.set_option(
        boost::asio::ip::multicast::join_group(multicast_address));

    socket.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&CIPTuner::handle_receive_from, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  
    io.run();
    
    return true;
}

bool CIPTuner::stopTune()
{
    socket.close();
    return true;
}
//boost::array<char, 128> recv_buf;

int main()
{
    CIPTuner iptuner;
    
    std::string addr = "234.1.1.1";
    unsigned short port = 5000;
    
    iptuner.startTune(addr, port);
    iptuner.stopTune();

#if 0
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: receiver <listen_address> <multicast_address>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 239.255.0.1\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 ff31::8000:1234\n";
      return 1;
    }

    boost::asio::io_service io_service;
    receiver r(io_service,
        boost::asio::ip::address::from_string(argv[1]),
        boost::asio::ip::address::from_string(argv[2]));
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
#endif    
    
    return 0;
}