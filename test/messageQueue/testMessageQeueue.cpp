#include "messageQueuePool.hpp"

int main()
{   
    auto pSharedPtr = CMessageQueuePool<int>::getInstance();
    pSharedPtr->createMessageQueue("heesoon.kim");
    
    CMessageQueue<int> *handlerOfmsgQ = pSharedPtr->getMessageQueue("heesoon.kim");
    if(handlerOfmsgQ == nullptr)
    {
        DEBUG_PRINT(DebugLevel4MessageQueuePool::DEBUG_LEVEL_NORMAL, "No Message handler Exist!!");
        return 0;
    }
    
    std::thread sender = std::thread([&](){
        for(int i = 0; i < 10; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            handlerOfmsgQ->sendMessage(i);
        }
    });
    
    std::thread receiver = std::thread([&](){
        while(1)
        {
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
            //int data = handlerOfmsgQ->receiveMessage();
            int data = 0;
            handlerOfmsgQ->receiveMessage(data);

            std::stringstream ss;
            std::string msg;

            ss << "received data : " << data << std::endl;
            msg = ss.str();

            DEBUG_PRINT(DebugLevel4MessageQueuePool::DEBUG_LEVEL_NORMAL, msg.c_str());
            if(data == 9)
                break;
        }
    });

    sender.join();
    receiver.join();
    
    return 0;
}
