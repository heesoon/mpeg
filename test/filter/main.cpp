#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include "sifilter.hpp"

int main(int argc, char *argv[])
{
#if 0   
    CTSfilter tsfilter;
    tsfilter.requestSection(0);
    tsfilter.requestSection(1);
    tsfilter.requestSection(48);
    tsfilter.requestSection(2);
    //for(int i = 0; i < MAX_FILTER_NUM; i++)
    {
        tsfilter.filterTaskId[0].join();
        tsfilter.filterTaskId[1].join();
        //tsfilter.filterTaskId[1].join();
        tsfilter.filterTaskId[48].join();
        tsfilter.filterTaskId[2].join();
    }
#endif
#if 0    
    std::ifstream fin;
    std::mutex mtx_lock;

    char buffer[188] = {0, };

    std::thread tr_send = std::thread([&](){

        fin.open("/home/hskim/share/fantasy_pi_pim.ts", std::ios_base::in | std::ios_base::binary);
        if(fin.is_open() == false)
        {
            std::cout << "file open error!!" << std::endl;
            return;
        }

        while(1)
        {
            mtx_lock.lock();
            fin.read(buffer, 188);
            mtx_lock.unlock();

            if(fin.gcount() == 0)
                break;
        }

    });

    std::thread tr_receive = std::thread([&](){

        CTSfilter filter;

        while(1)
        {
            mtx_lock.lock();
            mtx_lock.unlock();

            if(fin.gcount() == 0)
               break;
        }
    });    

    tr_send.join();
    tr_receive.join();
#endif

    CTSfilter filter;
    return 0;
}
