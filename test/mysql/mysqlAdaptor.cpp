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
	file : mysqlAdaptor.cpp
	mysql c++ connector abstration class, base class

	version : 1.0
	data    : 2016.06.01
	author  : HeeSoon Kim(chipmaker0304@gmail.com)
-----------------------------------------------------------------------*/

#include "mysqlAdaptor.h"

BaseMySQLAdaptor::BaseMySQLAdaptor()
{
	stmt 	= nullptr;
	con 	= nullptr;
	res 	= nullptr;

	driver = get_driver_instance();
	std::cout << __FUNCTION__ << std::endl;
}

BaseMySQLAdaptor::~BaseMySQLAdaptor()
{
	if(stmt != nullptr) delete stmt;
	if(con != nullptr) 	delete con;
	if(res != nullptr) 	delete res;
	
	std::cout << __FUNCTION__ << "," << __LINE__ << std::endl;
}

void BaseMySQLAdaptor::connect(const std::string address, const std::string user, const std::string password)
{
	con = driver->connect(address.c_str(), user.c_str(), password.c_str());
	stmt = con->createStatement();
}

void BaseMySQLAdaptor::createDB(const std::string databaseName)
{
	std::string databaseQuery = "CREATE DATABASE ";
	databaseQuery += databaseName;

	stmt->execute(databaseQuery.c_str());
}

void BaseMySQLAdaptor::useDB(const std::string databaseName)
{
	std::string databaseQuery = "USE ";
	databaseQuery += databaseName;

	stmt->execute(databaseQuery.c_str());
}

void BaseMySQLAdaptor::deleteDB(const std::string databaseName)
{
	std::string databaseQuery = "DROP DATABASE IF EXISTS ";
	databaseQuery += databaseName;

	stmt->execute(databaseQuery.c_str());
}

void BaseMySQLAdaptor::deleteTabe(const std::string tableName)
{
	std::string databaseQuery = "DROP TABLE IF EXISTS ";
	databaseQuery += tableName;

	//std::cout << databaseQuery << std::endl;
	stmt->execute(databaseQuery.c_str());
}