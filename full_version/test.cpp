#include <thread>
#include "section.h"
#include "pat.h"
#include "pmt.h"
#include "sdt.h"

int main(int argc, char* argv[])
{	
	SECTION_INFO_T sec;
	CSection* section = new CSection();
	PAT* pat = new PAT();
	PMT* pmt = new PMT();
	SDT* sdt = new SDT();

	// open media test, media is file
	section->openMedia("file://SGP.ts");

	// create task to read continously
	std::thread tr = std::thread([&]() {

		while(1)
		{
			if(section->readMedia(&sec) == RESULT_NOTOK)
				break;

			// PAT parsing PAT PID is zero
			if(sec.pid == 0)
			{
				pat->parsingPATSection((UINT8*)sec.section);
			}
			else if(pat->isPmtPid(sec.pid) == true)
			{
				// current pid is pmt pid
				pmt->parsingPMTSection(sec.pid, (UINT8*)sec.section);
			}
			else if(sdt->isSDTSecton(sec.pid) == true)
			{
				sdt->parsingSDTSection((UINT8*)sec.section);
			}			
		}
	});

	tr.join();

	// get PAT info
	pat->printPATList();
	pat->savePatToJson();

	// get PMT info
	pmt->printPMTList();
	pmt->savePmtToJson();
	
	// get SDT info
	sdt->printSDTList();
	sdt->saveSdtToJson();	

	delete section;
	delete pat;
	delete pmt;
	delete sdt;

	return 0;	
}