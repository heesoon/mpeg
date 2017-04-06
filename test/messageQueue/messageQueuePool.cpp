#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
#include <map>
#include <thread>
#include <chrono>

template <typename T>
class CMessageQueuePool;

template <typename T>
class CMessageQueue
{
    friend class CMessageQueuePool<T>;
private :
    std::queue<T> msgQ;
    std::mutex mtx;
    std::string nameOfmsgQ;
public  :
    CMessageQueue();
    virtual ~CMessageQueue();
    void sendMessage(T msg);
    T receiveMessage(void);
    bool isMessageQueueEmpty(void);
};

template <typename T>
CMessageQueue<T>::CMessageQueue()
{
    std::cout << __FUNCTION__ << std::endl;
}

template <typename T>
CMessageQueue<T>::~CMessageQueue()
{
    std::cout << __FUNCTION__ << std::endl;
    std::queue<T> empty;
    std::swap(empty, msgQ);
}

template <typename T>
void CMessageQueue<T>::sendMessage(T msg)
{
    std::cout << __FUNCTION__ << std::endl;
    
    mtx.lock();
    msgQ.push(msg);
    mtx.unlock();
}

template <typename T>
T CMessageQueue<T>::receiveMessage(void)
{
    std::cout << __FUNCTION__ << std::endl;
    T msg;
    
    mtx.lock();
    msg = msgQ.front();
    msgQ.pop();
    mtx.unlock();
    
    return msg;
}

template <typename T>
bool CMessageQueue<T>::isMessageQueueEmpty(void)
{
    std::cout << __FUNCTION__ << std::endl;
    
    bool isEmpty = false;
    
    mtx.lock();
    isEmpty = msgQ.empty();
    mtx.unlock();
    
    return isEmpty;
}

template <typename T>
class CMessageQueuePool
{
private :
    std::map<std::string, CMessageQueue<T>*> poolOfmsgQ;
    //using iterType = typename std::map<std::string, CMessageQueue<T>*>;
    CMessageQueuePool(){};    
    static std::shared_ptr<CMessageQueuePool<T>> inst;
public  :
    virtual ~CMessageQueuePool();
    static std::shared_ptr<CMessageQueuePool<T>> getInstance(void);
    bool createMessageQueue(std::string nameOfmsgQ);
    CMessageQueue<T>* getMessageQueue(std::string nameOfmsgQ);
};

template <typename T>
std::shared_ptr<CMessageQueuePool<T>> CMessageQueuePool<T>::inst = nullptr;

template <typename T>
std::shared_ptr<CMessageQueuePool<T>> CMessageQueuePool<T>::getInstance(void)
{
    std::cout << __FUNCTION__ << std::endl;
    if(inst == nullptr)
    {
        inst.reset(new CMessageQueuePool<T>);
    }
    
    return inst;
}

template <typename T>
CMessageQueuePool<T>::~CMessageQueuePool()
{
    std::cout << __FUNCTION__ << std::endl;
    
    for(auto iter = poolOfmsgQ.begin(); iter != poolOfmsgQ.end(); iter++)
    {
        delete iter->second;
    }
    
    poolOfmsgQ.clear();
}

template <typename T>
bool CMessageQueuePool<T>::createMessageQueue(std::string nameOfmsgQ)
{
    std::cout << __FUNCTION__ << std::endl;
    
    if(poolOfmsgQ.find(nameOfmsgQ) != poolOfmsgQ.end())
    {
        std::cout << "Already Message Queue Exist!!" << std::endl;
        return false;
    }
    
    CMessageQueue<T> *newMessageQueue   = new CMessageQueue<T>;
    newMessageQueue->nameOfmsgQ         = nameOfmsgQ;    
    poolOfmsgQ[nameOfmsgQ]              = newMessageQueue;
        
    return true;
}

template <typename T>
CMessageQueue<T>* CMessageQueuePool<T>::getMessageQueue(std::string nameOfmsgQ)
{
    std::cout << __FUNCTION__ << std::endl;

    for(auto iter = poolOfmsgQ.begin(); iter != poolOfmsgQ.end(); iter++)
    {
        if(iter->first.compare(nameOfmsgQ) == 0)
        {
            return iter->second;
        }
    }
    
    return nullptr;
}

int main()
{   
    auto pSharedPtr = CMessageQueuePool<int>::getInstance();
    pSharedPtr->createMessageQueue("heesoon.kim");
    
    CMessageQueue<int> *handlerOfmsgQ = pSharedPtr->getMessageQueue("heesoon.kim");
    if(handlerOfmsgQ == nullptr)
    {
        std::cout << "No Message handler Exist!!" << std::endl;
        return 0;
    }
    
    std::thread sender = std::thread([&](){
        for(int i = 0; i < 10; i++)
        {
            handlerOfmsgQ->sendMessage(i);
        }
    });
    
    std::thread receiver = std::thread([&](){
        while(1)
        {
            if(handlerOfmsgQ->isMessageQueueEmpty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
            int data = handlerOfmsgQ->receiveMessage();
            std::cout << "recevie data : " << data << std::endl;
            if(data == 9)
                break;
        }
    });

    sender.join();
    receiver.join();
    
    return 0;
}