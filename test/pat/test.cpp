#include <thread>
#include "section.h"
#include "pat.h"

int main(int argc, char* argv[])
{	
	SECTION_INFO_T sec;
	CSection* section = new CSection();
	PAT* pat = new PAT();

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
		}
	});

	tr.join();

	// print PAT info
	pat->printPATList();

	delete section;
	delete pat;

	return 0;	
}