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

#ifndef __MSGQ_H__
#define __MSGQ_H__
#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <condition_variable>

template <typename T>
class CMsgQ {
public:
    CMsgQ(const std::string& n);
    virtual void sendMsg(const T& msg);
    virtual T receiveMsg(void);
    virtual bool isMsgQEmpty(void);
    virtual ~CMsgQ();
private:
    std::queue<T> msgQ;
    std::mutex mtx;
    std::string qName;
    std::condition_variable cv;
};

template <typename T>
CMsgQ<T>::CMsgQ(const std::string& n) : qName(n) {

}

template <typename T>
void CMsgQ<T>::sendMsg(const T& msg) {
    std::unique_lock<std::mutex> lk(mtx);
    msgQ.push(msg);

    cv.notify_all();
}

template <typename T>
T CMsgQ<T>::receiveMsg(void) {
    T msg;
    std::unique_lock<std::mutex> lk(mtx);

    if(isMsgQEmpty()) {
        cv.wait(lk);
    }
    msg = msgQ.front();
    msgQ.pop();

    return msg;
}

template <typename T>
bool CMsgQ<T>::isMsgQEmpty() {
    return msgQ.empty();
}

template <typename T>
CMsgQ<T>::~CMsgQ() {
    std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
}

template <typename T>
class CMsgQManager {
public:
    virtual std::shared_ptr<CMsgQ<T>> createMsgQ(const std::string& n);
    //virtual void removeMsgQ(const std::string& n);
    virtual  std::shared_ptr<CMsgQ<T>> getMsgQ(const std::string& n);
    virtual ~CMsgQManager();
    static CMsgQManager<T>& getInstance() {
    	static CMsgQManager<T> instance;
    	return instance;
    }
private:
	CMsgQManager() {}
	CMsgQManager(const CMsgQManager& c) = delete;
	void operator=(const CMsgQManager& c) = delete;
    std::unordered_map<std::string, std::shared_ptr<CMsgQ<T>>> um;
};

template <typename T>
std::shared_ptr<CMsgQ<T>> CMsgQManager<T>::createMsgQ(const std::string& n) {
    auto search = um.find(n);
    if(search != um.end()) {
        std::cout << "error : duplicate msgQ ... " << std::endl;
        return search->second;
    }

    std::shared_ptr<CMsgQ<T>> tsp = std::make_shared<CMsgQ<T>>(n);
    um[n] = tsp;
    
    return tsp;
}

template <typename T>
std::shared_ptr<CMsgQ<T>> CMsgQManager<T>::getMsgQ(const std::string& n) {
    auto search = um.find(n);
    if(search == um.end()) {
        std::cout << "error : can't find msgQ... " << std::endl;
        return nullptr;
    }

    return search->second;
}

template <typename T>
CMsgQManager<T>::~CMsgQManager() {
    std::cout << __FUNCTION__ << ", " << __LINE__ << std::endl;
}
#endif