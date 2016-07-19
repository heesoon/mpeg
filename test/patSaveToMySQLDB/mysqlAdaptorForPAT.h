#ifndef __MYSQL_ADAPTOR_PAT_H__
#define __MYSQL_ADAPTOR_PAT_H__

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <pat.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "mysqlAdaptor.h"
/* -------------------------------------------------------------------------------------------------------------------
	declare define
------------------------------------------------------------------------------------------------------------------- */
// http://blog.naver.com/PostView.nhn?blogId=fyman&logNo=140069146533
// https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-examples-complete-example-1.html

/* -------------------------------------------------------------------------------------------------------------------
	declare class data
------------------------------------------------------------------------------------------------------------------- */
class MySQLAdaptorForPAT : public BaseMySQLAdaptor
{
public :
	MySQLAdaptorForPAT();
	virtual ~MySQLAdaptorForPAT();
	void createTable(const std::string databaseName, const std::string tableName);
	void insertTable(const std::string tableName, void *object);
	void select(const std::string tableName);
};

#endif