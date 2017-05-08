#include <iostream>
#include <memory>
#include "messageQueuePool.hpp"
#include "sifilter.hpp"
#include "iptuner.hpp"

int main(int argc, char* argv[])
{
	std::shared_ptr<CIPTuner> iptuner = std::make_shared<CIPTuner>();
	CTSfilterManager *pSifilterMgr = CTSfilterManager::getInstance();

	pSifilterMgr->closeTSFilterMgr();

    return 0;
}