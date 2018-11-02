//
// Created by sandy on 31/10/18.
//

#include "DBManager.h"
#include <chrono>
#include <thread>
#include <math.h>
#include <cstring>
#include <thread>

DBManager* DBManager::pSingleton= NULL;


DBManager* DBManager::GetInstance()
{
	if (pSingleton== NULL) {
		pSingleton = new DBManager();
	}
	return pSingleton;
}

DBManager::DBManager()
{
	std::cout << "Constructor Invoked" << std::endl;
	connect();
	createSRTable();
	createDataTable();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //Ensure that Table is created successfully
	std::thread executorThread(&DBManager::sqlExecutor,this);
	executorThread.detach();
}

bool DBManager::isBusy()
{
	if (sql_executionQueue.size() > 0 || sqlite3_close(db)==SQLITE_BUSY)
		return true;
	else
		return false;

}


int DBManager::connect()
{
	rc = sqlite3_open(dbName, &db);
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return (100);
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
		return (0);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //Ensure that Table is created successfully
}

void DBManager::createSRTable()
{


	char *sql = "CREATE TABLE  IF NOT EXISTS`SRDATA` (						\
				`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,	\
				`PID`	TEXT NOT NULL,										\
				`LOC`	INTEGER NOT NULL,									\
				`CAL_VAL`	INTEGER NOT NULL,								\
				`Timestamp` DATETIME DEFAULT CURRENT_TIMESTAMP				);";
	queueSQL(sql, false);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //Ensure that Table is created successfully
}

void DBManager::sqlExecutor()
{
	/* Execute SQL statement */
	//
	while(1)
	{
		if (sql_executionQueue.size() == 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		else
		{
			char *sql = sql_executionQueue.front();
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

			if (rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			} else
			{
				fprintf(stdout, "SQL Executed Successfully\n");
			}
			sql_executionQueue.pop();
		}
	}
}

void DBManager::createDataTable()
{

	char *sql = "CREATE TABLE IF NOT EXISTS `MAINDATA` (					\
				`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,	\
				`ELAPSED`	INTEGER NOT NULL,								\
				`PID`	TEXT NOT NULL,										\
				`CONDITION`	TEXT NOT NULL,									\
				`TEST_SP`	INTEGER NOT NULL,								\
				`S_TOUCHED`	INTEGER NOT NULL,								\
				`FORCES`	TEXT NOT NULL,									\
				`CONFIRM_PRESSED`	TEXT NOT NULL,							\
 				`Timestamp` DATETIME DEFAULT CURRENT_TIMESTAMP				\
				);";

	queueSQL(sql, false);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //Ensure that Table is created successfully
}



int DBManager::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


void DBManager::writeMainData(float elapsed, char *pid, char *condition, int test_sp, int s_touched, char *forces, char *confirm_press)
{
	char elapsedBuffer[20];
	char test_spBuffer[20];
	char s_touchedBuffer[20];
	sprintf(elapsedBuffer, "%.1f", roundf(elapsed * 100) / 100);
	sprintf(test_spBuffer, "%d", test_sp);
	sprintf(s_touchedBuffer, "%d", s_touched);
	std::string sql  =	"INSERT INTO `MAINDATA`(`ELAPSED`,`PID`,`CONDITION`,`TEST_SP`,`S_TOUCHED`,`FORCES`,`CONFIRM_PRESSED`) VALUES (";
						sql.append("");		sql.append(elapsedBuffer); 		sql.append(",");
						sql.append("'");	sql.append(pid); 				sql.append("',");
						sql.append("'");	sql.append(condition); 			sql.append("',");
						sql.append("");		sql.append(test_spBuffer); 		sql.append(",");
						sql.append("");		sql.append(s_touchedBuffer); 	sql.append(",");
						sql.append("'");	sql.append(forces); 			sql.append("',");
						sql.append("'");	sql.append(confirm_press); 		sql.append("');");
	char *csql = new char[sql.length() + 1];
	strcpy(csql, sql.c_str());
	queueSQL(csql, true);

}


void DBManager::writeSRData(char *pid, int location, int cal_val)
{
	char locationBuffer[20];
	char cal_valBuffer[20];
	sprintf(locationBuffer, "%d", location);
	sprintf(cal_valBuffer, "%d", cal_val);
	std::string sql  =	"INSERT INTO `SRDATA`(`PID`,`LOC`,`CAL_VAL`) VALUES (";
	sql.append("'");	sql.append(pid); 				sql.append("',");
	sql.append("");		sql.append(locationBuffer); 	sql.append(",");
	sql.append("");	sql.append(cal_valBuffer); 			sql.append(");");
	char *csql = new char[sql.length() + 1];
	strcpy(csql, sql.c_str());
	queueSQL(csql, true);

}

void DBManager::queueSQL(char *sql, bool usedNew)
{
	sql_executionQueue.push(sql);
}