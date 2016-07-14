#include <iostream>
#include "mysqlAdaptor.h"

class BaseMySQLAdaptorTest : public BaseMySQLAdaptor
{
public :
	BaseMySQLAdaptorTest(){
		std::cout << __FUNCTION__ << "," << __LINE__ << std::endl;
	};
	virtual ~BaseMySQLAdaptorTest(){
		std::cout << __FUNCTION__ << "," << __LINE__ << std::endl;
	};
	void createTable(const std::string databaseName, const std::string tableName){
		std::cout << __FUNCTION__ << ", ";
		std::cout << __LINE__ << " ";
		std::cout << "called " << std::endl;
	};
	void insertTable(const std::string tableName, void *object){
		std::cout << __FUNCTION__ << ", ";
		std::cout << __LINE__ << " ";
		std::cout << "called " << std::endl;
	};
	void select(const std::string tableName){
		std::cout << __FUNCTION__ << ", ";
		std::cout << __LINE__ << " ";
		std::cout << "called " << std::endl;
	};
};

int main(int argc, char* argv[])
{
	std::string address 	= "tcp://127.0.0.1:3306";
	std::string user 		= "root";
	std::string password 	= "";
	std::string dbName 		= "test";
	std::string tableName 	= "pat";

	BaseMySQLAdaptorTest *pMySqlTest = new BaseMySQLAdaptorTest();
	pMySqlTest->connect(address, user, password);
	pMySqlTest->deleteDB(dbName);
	pMySqlTest->createDB(dbName);
	pMySqlTest->useDB(dbName);

	// only for testing temporally to implement virtual function
	pMySqlTest->createTable(dbName, tableName);
	pMySqlTest->insertTable(dbName, nullptr);
	pMySqlTest->select(tableName);

	delete pMySqlTest;

	return 0;
}