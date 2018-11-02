//
// Created by sandy on 31/10/18.
//

#ifndef CLIONSQL_DBMANAGER_H
#define CLIONSQL_DBMANAGER_H
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <queue>

class DBManager
{
private:
	sqlite3 *db;
	const char* dbName = "test.db";
	int rc;
	char *zErrMsg = 0;
	int connect();
	void createSRTable();
	void createDataTable();
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	void sqlExecutor();
	void queueSQL(char *sql, bool usedNew);
	std::queue<char*> sql_executionQueue;
	DBManager();
	static DBManager* pSingleton;		// singleton instance

public:
	static DBManager* GetInstance();
	void writeMainData(float elapsed, char *pid, char *condition, int test_sp, int s_touched, char *forces, char *confirm_press);
	void writeSRData(char *pid, int location, int cal_val);
	bool isBusy();


};


#endif //CLIONSQL_DBMANAGER_H
