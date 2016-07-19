#include <thread>
#include <chrono>
#include <mutex>
#include "section.h"
#include "pat.h"
#include "mysqlAdaptorForPAT.h"

int main(int argc, char* argv[])
{	
	std::string address 	= "tcp://127.0.0.1:3306";
	std::string user 		= "root";
	std::string password 	= "";
	std::string dbName 		= "test";
	std::string tableName 	= "pat";

	SECTION_INFO_T sec;
	CSection* section 				= new CSection();
	PAT* pat 						= new PAT();
	MySQLAdaptorForPAT *pMySqlPAT 	= new MySQLAdaptorForPAT();

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

	pMySqlPAT->connect(address, user, password);
	pMySqlPAT->deleteDB(dbName);
	pMySqlPAT->createDB(dbName);
	pMySqlPAT->createTable(dbName, tableName);
	pMySqlPAT->insertTable(tableName, (void*)pat);
	pMySqlPAT->select(tableName);

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	delete section;
	delete pat;
	delete pMySqlPAT;	

	return 0;	
}