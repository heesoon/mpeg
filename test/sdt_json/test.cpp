#include <thread>
#include "section.h"
#include "sdt.h"

int main(int argc, char* argv[])
{	
	SECTION_INFO_T sec;
	CSection* section 	= new CSection();
	SDT* sdt 			= new SDT();

	// open media test, media is file
	section->openMedia("file://SGP.ts");

	// create task to read continously
	std::thread tr = std::thread([&]() {

		while(1)
		{
			if(section->readMedia(&sec) == RESULT_NOTOK)
				break;

			// check SDT section
			if(sdt->isSDTSecton(sec.pid) == true)
			{
				sdt->parsingSDTSection((UINT8*)sec.section);
			}
		}
	});

	tr.join();

	sdt->printSDTList();
	sdt->saveSdtToJson();

	delete section;
	delete sdt;

	return 0;	
}