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

/** --------------------------------------------------------------------
	file : mysqlAdaptorForPAT.cpp
	mysql c++ connector wrapper class

	version : 1.0
	data    : 2016.06.01
	author  : HeeSoon Kim(chipmaker0304@gmail.com)
-----------------------------------------------------------------------*/

#include "mysqlAdaptorForPAT.h"
#include "pat.h"

std::string PatTableString[] = {
	"version INT,",
	"section_number INT,",
	"program_number INT,",
	"pmtPid INT"
};

MySQLAdaptorForPAT::MySQLAdaptorForPAT()
{
	driver = get_driver_instance();
	std::cout << __FUNCTION__ << std::endl;
}

MySQLAdaptorForPAT::~MySQLAdaptorForPAT()
{
	std::cout << __FUNCTION__ << "," << __LINE__ << std::endl;
}

void MySQLAdaptorForPAT::createTable(const std::string databaseName, const std::string tableName)
{
	useDB(databaseName);

	std::string columQuery;

	for(auto str : PatTableString)
	{
		columQuery += str;
	}

	std::string databaseQuery = "CREATE TABLE ";
	databaseQuery += tableName;
	databaseQuery += "(";
	databaseQuery += columQuery;
	databaseQuery += ")";

	deleteTabe(tableName);
	stmt->execute(databaseQuery.c_str());
}

void MySQLAdaptorForPAT::insertTable(const std::string tableName, void *object)
{
	PAT *pat;
	std::string databaseQuery;

	pat = static_cast<PAT*> (object);

	for(auto list : pat->getPatList())
	{
		char buffer[256] = {0, };

		std::sprintf(buffer, "(%d, %d, %d, %d)", list.version, list.section_number, list.program_number,\
			list.pmtPid);

		databaseQuery = "INSERT INTO ";
		databaseQuery += tableName;
		databaseQuery += " VALUES";
		databaseQuery += buffer;

		stmt->execute(databaseQuery.c_str());
	}
}

void MySQLAdaptorForPAT::select(const std::string tableName)
{
	std::string databaseQuery;
	int i = 1;

	databaseQuery += "SELECT * from ";
	databaseQuery += tableName;

	res = stmt->executeQuery(databaseQuery);

	while (res->next()) 
	{
		std::cout << std::dec << i << "] " << "PAT ";
		std::cout << "version : " << res->getString(1) << ", ";
		std::cout << "section_number : " << res->getString(2) << ", ";
		std::cout << "program_number : " << res->getString(3) << ", ";
		std::cout << "PID : " << res->getString(4) << " ";
		std::cout << "[ " << std::hex << std::showbase << res->getInt(4) << " ]" << std::endl;
		i++;

	}		
}
