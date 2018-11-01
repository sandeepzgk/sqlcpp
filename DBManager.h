//
// Created by sandy on 31/10/18.
//

#ifndef CLIONSQL_DBMANAGER_H
#define CLIONSQL_DBMANAGER_H
#include <sqlite3.h>
#include <iostream>
#include <string>

class DBManager
{
private:
	sqlite3 *db;
	const char* dbName = "test.db";
	int rc;
	char *zErrMsg = 0;
	int connect();
	int disconnect();
	void createSRTable();
	void createDataTable();
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	void sqlExecutor(char *sql,bool usedNew);
	void detachableExecutor(char *sql,bool usedNew);
	int threadExecuting = 0;
public:
	DBManager();
	~DBManager();
	void writeMainData(float elapsed, char *pid, char *condition, int test_sp, int s_touched, char *forces, char *confirm_press);
	void writeSRData(char *pid, int location, int cal_val);


};


#endif //CLIONSQL_DBMANAGER_H
