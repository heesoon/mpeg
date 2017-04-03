#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

template <typename T>
class CMessageQueue
{
private :
    unsigned int numOfQid;
    class CMessageQueueInfo
    {
    public  :
        int msgQId;
        std::queue<T> msgQ;
        std::mutex mtx;
        std::string msgQName;
        CMessageQueueInfo(){std::cout << __FUNCTION__ << std::endl;}
        ~CMessageQueueInfo(){
            std::cout << __FUNCTION__ << std::endl;
            std::queue<T> empty;
            std::swap(msgQ, empty);
        }
    };
    
    std::vector<CMessageQueueInfo*> vMessageQueuePool;
public  :
    CMessageQueue();
    virtual ~CMessageQueue();
    bool createMessageQueue(std::string name);
    std::queue<T>* getMessageQueueHandleByMsgQName(std::string name);
    std::mutex* getMessageQueueMutexHandleByMsgQName(std::string name);
    void pushMessageQueue(std::queue<T> *msgQ, std::mutex *mtx, T &data);
    T popMessageQueue(std::queue<T> *msgQ, std::mutex *mtx);
    bool isMessageQueueEmpty(std::queue<T> *msgQ, std::mutex *mtx);
    void printMessageQueuePool(void);
};

template <typename T>
CMessageQueue<T>::CMessageQueue()
{
    std::cout << __FUNCTION__ << std::endl;
    numOfQid = 0;
}

template <typename T>
CMessageQueue<T>::~CMessageQueue()
{
    std::cout << __FUNCTION__ << std::endl;
    
    for(CMessageQueueInfo *msgQInfo : vMessageQueuePool)
    {
        delete msgQInfo;
        std::cout << "delete queue info" << std::endl;
    }
}

template <typename T>
bool CMessageQueue<T>::createMessageQueue(std::string name)
{
    if(name == " ")
    {
        std::cout << "No Message Queue Name!!" << std::endl;
        return false;
    }

    CMessageQueueInfo *newMessageQueue = new CMessageQueueInfo;
    if(newMessageQueue == nullptr)
    {
        std::cout << "create new message queue fail!!" << std::endl;
        return false;
    }
    
    newMessageQueue->msgQName   = name;
    newMessageQueue->msgQId     = ++numOfQid;
    
    vMessageQueuePool.push_back(newMessageQueue);
    
    return true;
}

template <typename T>
std::queue<T>* CMessageQueue<T>::getMessageQueueHandleByMsgQName(std::string name)
{
    for(CMessageQueueInfo *q : vMessageQueuePool)
    {
        if(q->msgQName.compare(name) == 0)
        {
            return &q->msgQ;
        }
    }

    return nullptr;
}

template <typename T>
std::mutex* CMessageQueue<T>::getMessageQueueMutexHandleByMsgQName(std::string name)
{
    for(CMessageQueueInfo *q : vMessageQueuePool)
    {
        if(q->msgQName.compare(name) == 0)
        {
            return &q->mtx;
        }
    }

    return nullptr;
}

template <typename T>
bool CMessageQueue<T>::isMessageQueueEmpty(std::queue<T> *msgQ, std::mutex *mtx)
{
    bool isEmpty = false;

    if(msgQ == nullptr)
    {
        std::cout << "No Message Queue Handle!!" << std::endl;
        return true;
    }

    //msgQ->mtx.lock();
    mtx->lock();
    isEmpty = msgQ->empty();
    //msgQ->mtx.unlock();        
    mtx->unlock();

    return isEmpty;
}

template <typename T>
void CMessageQueue<T>::pushMessageQueue(std::queue<T> *msgQ, std::mutex *mtx, T &data)
{
    if(msgQ == nullptr)
    {
        std::cout << "No Message Queue Handle!!" << std::endl;
        return;
    }

    //msgQ->mtx.lock();
    mtx->lock();
    msgQ->push(data);
    //msgQ->mtx.unlock();
    mtx->unlock();
}

template <typename T>
T CMessageQueue<T>::popMessageQueue(std::queue<T> *msgQ, std::mutex *mtx)
{
    T data;
    if(msgQ == nullptr)
    {
        std::cout << "No Message Queue Handle!!" << std::endl;
        return 0;
    }

    if(mtx == nullptr)
    {
        std::cout << "No Message Queue Mutex Handle!!" << std::endl;
        return 0;
    }

#if 0
    if(msgQ->empty() == true)
    {
       std::cout << "No Message Queue Mutex Handle!!" << std::endl;
        return 0;        
    }
#endif
    //msgQ->mtx.lock();
    mtx->lock();
    data = msgQ->front();
    msgQ->pop();
    //msgQ->mtx.unlock();    
    mtx->unlock();

    return data;
}

template <typename T>
void CMessageQueue<T>::printMessageQueuePool(void)
{
    std::cout << __FUNCTION__ << std::endl;
    for(CMessageQueueInfo *q : vMessageQueuePool)
    {
        std::cout << q->msgQName << std::endl;
        std::cout << q->msgQId << std::endl;
        
        while(!q->msgQ.empty())
        {
            std::cout << q->msgQ.front() << std::endl;
        }
    }
}

int main()
{
    std::shared_ptr<CMessageQueue<int>> pSharedMsgQPool = std::make_shared<CMessageQueue<int>>();
    pSharedMsgQPool->createMessageQueue("msgQueueTest");

    // sender
    std::thread tr1 = std::thread([&](){
        std::queue<int> *sender = pSharedMsgQPool->getMessageQueueHandleByMsgQName("msgQueueTest");
        std::mutex *mtx = pSharedMsgQPool->getMessageQueueMutexHandleByMsgQName("msgQueueTest");
        if(sender == nullptr)
        {
            return;
        }
            
        for(int i = 0; i < 100; i++)
        {
            pSharedMsgQPool->pushMessageQueue(sender, mtx, i); 
        }
    });
    
    // receiver
    std::thread tr2 = std::thread([&](){
        int i = 0;
        std::queue<int> *receiver = pSharedMsgQPool->getMessageQueueHandleByMsgQName("msgQueueTest");
        std::mutex *mtx = pSharedMsgQPool->getMessageQueueMutexHandleByMsgQName("msgQueueTest");
        if(receiver == nullptr)
        {
            return;
        }

        while(i++ < 200)
        {
            if(pSharedMsgQPool->isMessageQueueEmpty(receiver, mtx) == true)
            {
                std::this_thread::sleep_for( std::chrono::milliseconds(10) );
                continue;
            }

            std::cout << "receive data : " << pSharedMsgQPool->popMessageQueue(receiver, mtx) << std::endl;
            //pSharedMsgQPool->popMessageQueue(receiver, mtx);
            //receiver->pop();
        }
    });    
    
    tr1.join();
    tr2.join();

    std::cout << "end of program !!" << std::endl;
    
    return 0;
}