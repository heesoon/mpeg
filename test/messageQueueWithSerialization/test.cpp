#include <iostream>
#include <string>
#include <thread>
#include <cstdio>
#include <sstream>
#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp> 
#include <boost/archive/binary_iarchive.hpp> 
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/serialization/string.hpp>

using namespace boost::archive;
using namespace boost::interprocess;

#define MAX_SIZE 1000

class Message 
{
public :
		Message(std::string aname, int aage) : name(aname), age(aage) {};
		void print(void)
		{
			std::cout << "name : " << name << ", ";
			std::cout << "age : " << age << std::endl;
		};

private :
	friend class boost::serialization::access; 

	template <typename Archive> 
	void serialize(Archive &ar, const unsigned int version) 
	{ 
		ar &name;
		ar &age;

		std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
	} 

	std::string name;
    int age;
};

int main(int argc, char* argv[])
{
    int i = 0;
    char ch;

    Message smessage("heesoon.kim", 41);
    Message rmessage("", 0);

    std::string msgQName    = "test";

     //Erase previous message queue
    message_queue::remove(msgQName.c_str());

     //Create a message_queue.
    message_queue mqSend
        //(open_or_create               //create or open
        (create_only                    //create only
        ,msgQName.c_str()               //name
        ,1                              //max message number
        //,sizeof(smessage)               //max message size
        ,MAX_SIZE
    );

    message_queue mqReceive
        (open_only          //only create
        ,msgQName.c_str()   //name
    );

    try
    {
    	// task sending message
        std::thread tr_send = std::thread([&]() {
 
            while(i++ < 10)
            {
            	std::stringstream oss;
        		boost::archive::text_oarchive oa(oss);

            	oa << smessage; // serilize smessage to oss buffer
                std::string serialized_string(oss.str()); // send message by message queue with string buffer
        		mqSend.send(serialized_string.data(), serialized_string.size(), 0);
            }
         });

        // task receiving message from above task
        std::thread tr_receive = std::thread([&]() {
 
            unsigned int priority;
            message_queue::size_type recvd_size;

            while(1)
            {
            	std::stringstream iss;
	        	std::string serialized_string;
	        	serialized_string.resize(MAX_SIZE);

		        mqReceive.receive(&serialized_string[0], MAX_SIZE, recvd_size, priority);
		        serialized_string.resize(recvd_size);
		        iss << serialized_string; // save received serialized data to streamstring buffer 
		        boost::archive::text_iarchive ia(iss);
		        ia >> rmessage; // de-serialization received data

		        rmessage.print();
            }
        });

        tr_send.detach();
        tr_receive.detach();
    }
    catch(interprocess_exception &ex){
        std::cerr << ex.what() << std::endl;
        return 1;
    }   
 
    while(1)
    {
        std::cout << "In main thread" << std::endl;
 
        std::cin.get(ch);
        if(ch == 'x' || ch == 'X')
            break;
    }
 
    message_queue::remove(msgQName.c_str());
 
    return 0;
}