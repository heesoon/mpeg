#ifndef __BASE_MYSQL_ADAPTOR_H__
#define __BASE_MYSQL_ADAPTOR_H__

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
/* -------------------------------------------------------------------------------------------------------------------
	declare define
------------------------------------------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------------------------------------------
	declare class data
------------------------------------------------------------------------------------------------------------------- */
class BaseMySQLAdaptor
{
public :
	BaseMySQLAdaptor();
	virtual ~BaseMySQLAdaptor();
	void connect(const std::string address, const std::string user, const std::string password);
	void createDB(const std::string databaseName);
	void useDB(const std::string databaseName);
	void deleteDB(const std::string databaseName);
	void deleteTabe(const std::string tableName);
	virtual void createTable(const std::string databaseName, const std::string tableName) = 0;
	virtual void insertTable(const std::string tableName, void *object) = 0;
	virtual void select(const std::string tableName) = 0;

protected :
	sql::Driver *driver;
	sql::Connection	*con;
	sql::Statement	*stmt;
	sql::ResultSet *res;
};
#endif