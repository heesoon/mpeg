#include <thread>
#include "section.h"

int main(int argc, char* argv[])
{	
	SECTION_INFO_T sec;
	CSection* section = new CSection();

	// open media test, media is file
	section->openMedia("file://SGP.ts");

	// create task to read continously
	std::thread tr = std::thread([&]() {

		while(1)
		{
			if(section->readMedia(&sec) == RESULT_NOTOK)
				break;
		}
	});

	tr.join();

	delete section;

	return 0;	
}