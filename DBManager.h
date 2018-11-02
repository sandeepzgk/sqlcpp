//
// Created by sandy on 31/10/18.
//

#ifndef CLIONSQL_DBMANAGER_H
#define CLIONSQL_DBMANAGER_H

#include <sqlite3.h>
#include <iostream>
#include <queue>
#include <math.h>
#include <cstring>
#include <thread>


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
	void createForceTable();
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	void sqlExecutor();
	void queueSQL(std::string);
	std::queue<std::string> sql_executionQueue;
	DBManager();
	static DBManager* pSingleton;		// singleton instance

public:
	static DBManager* GetInstance();
	void writeMainData(float elapsed, std::string pid, std::string condition, int test_sp, int s_touched,
					   std::string confirm_press);
	void writeSRData(std::string pid, int location, int cal_val);
	void writeForceData(std::string pid, int s_touched, float f_x, float f_y, float f_z);
	bool isBusy();


};


#endif //CLIONSQL_DBMANAGER_H
