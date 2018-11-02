//
// Created by sandy on 31/10/18.
//

#include "DBManager.h"

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
	createForceTable();
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
}

void DBManager::createSRTable()
{


	std::string sql = "CREATE TABLE  IF NOT EXISTS`SRDATA` (						\
				`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,	\
				`PID`	TEXT NOT NULL,										\
				`LOC`	INTEGER NOT NULL,									\
				`CAL_VAL`	INTEGER NOT NULL,								\
				`Timestamp` DATETIME DEFAULT CURRENT_TIMESTAMP				);";
	queueSQL(sql);
}

void DBManager::sqlExecutor()
{
	while(1)
	{
		if (sql_executionQueue.size() == 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		else
		{
			std::string sqlString = sql_executionQueue.front();
			char *csql = new char[sqlString.length() + 1];
			strcpy(csql, sqlString.c_str());
			rc = sqlite3_exec(db, csql, callback, 0, &zErrMsg);

			if (rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			sql_executionQueue.pop();
			delete[] csql;
		}
	}
}

void DBManager::createDataTable()
{

	std::string sql = "CREATE TABLE IF NOT EXISTS `MAINDATA` (					\
				`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,	\
				`ELAPSED`	INTEGER NOT NULL,								\
				`PID`	TEXT NOT NULL,										\
				`CONDITION`	TEXT NOT NULL,									\
				`TEST_SP`	INTEGER NOT NULL,								\
				`S_TOUCHED`	INTEGER NOT NULL,								\
				`CONFIRM_PRESSED`	TEXT NOT NULL,							\
 				`Timestamp` DATETIME DEFAULT CURRENT_TIMESTAMP				\
				);";

	queueSQL(sql);
}

void DBManager::createForceTable()
{

	std::string sql = "CREATE TABLE IF NOT EXISTS `FORCEDATA` (					\
				`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,	\
				`PID`	TEXT NOT NULL,										\
				`S_TOUCHED`	INTEGER NOT NULL,								\
				`F_X`	TEXT NOT NULL,										\
				`F_Y`	TEXT NOT NULL,										\
				`F_Z`	TEXT NOT NULL,										\
 				`Timestamp` DATETIME DEFAULT CURRENT_TIMESTAMP				\
				);";

	queueSQL(sql);
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


void DBManager::writeMainData(float elapsed, std::string pid, std::string condition, int test_sp, int s_touched,
							  std::string confirm_press)
{
	char elapsedBuffer[20];
	char test_spBuffer[20];
	char s_touchedBuffer[20];
	sprintf(elapsedBuffer, "%.3f", roundf(elapsed * 1000) / 1000);
	sprintf(test_spBuffer, "%d", test_sp);
	sprintf(s_touchedBuffer, "%d", s_touched);
	std::string sql  =	"INSERT INTO `MAINDATA`(`ELAPSED`,`PID`,`CONDITION`,`TEST_SP`,`S_TOUCHED`,`CONFIRM_PRESSED`) VALUES (";
						sql.append("");		sql.append(elapsedBuffer); 		sql.append(",");
						sql.append("'");	sql.append(pid); 				sql.append("',");
						sql.append("'");	sql.append(condition); 			sql.append("',");
						sql.append("");		sql.append(test_spBuffer); 		sql.append(",");
						sql.append("");		sql.append(s_touchedBuffer); 	sql.append(",");
						sql.append("'");	sql.append(confirm_press); 		sql.append("');");

	queueSQL(sql);

}



void DBManager::writeForceData(std::string pid, int s_touched, float f_x, float f_y, float f_z)
{
	char s_touchedBuffer[20];
	char f_xBuffer[20];
	char f_yBuffer[20];
	char f_zBuffer[20];
	sprintf(s_touchedBuffer, "%d", s_touched);
	sprintf(f_xBuffer, "%.3f", roundf(f_x * 1000) / 1000);
	sprintf(f_yBuffer, "%.3f", roundf(f_y * 1000) / 1000);
	sprintf(f_zBuffer, "%.3f", roundf(f_z * 1000) / 1000);
	std::string sql  =	"INSERT INTO `FORCEDATA`(`PID`,`S_TOUCHED`,`F_X`,`F_Y`,`F_Z`) VALUES (";
	sql.append("'");	sql.append(pid); 				sql.append("',");
	sql.append("");		sql.append(s_touchedBuffer); 	sql.append(",");
	sql.append("'");	sql.append(f_xBuffer); 			sql.append("',");
	sql.append("'");	sql.append(f_yBuffer); 			sql.append("',");
	sql.append("'");	sql.append(f_zBuffer); 			sql.append("');");
	queueSQL(sql);

}


void DBManager::writeSRData(std::string pid, int location, int cal_val)
{
	char locationBuffer[20];
	char cal_valBuffer[20];
	sprintf(locationBuffer, "%d", location);
	sprintf(cal_valBuffer, "%d", cal_val);
	std::string sql  =	"INSERT INTO `SRDATA`(`PID`,`LOC`,`CAL_VAL`) VALUES (";
	sql.append("'");	sql.append(pid); 				sql.append("',");
	sql.append("");		sql.append(locationBuffer); 	sql.append(",");
	sql.append("");	sql.append(cal_valBuffer); 			sql.append(");");
	queueSQL(sql);
}

void DBManager::queueSQL(std::string sql)
{
	sql_executionQueue.push(sql);
}