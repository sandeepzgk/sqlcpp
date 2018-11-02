#include "DBManager.h"
#include <thread>

int main() {

    DBManager *dbmanager = DBManager::GetInstance();
	for (int i = 0; i < 100; i++)
	{
		dbmanager->writeMainData(5.5, "P1", "A", 3, 2,  "Y");
		dbmanager->writeSRData("P1",1,12);
		dbmanager->writeForceData("Og81",12,1.24,15.55,12.44);
		std::cout<<i+1<<std::endl;
	}
	while(dbmanager->isBusy())
	{
		std::cout<<"DBManager is busy"<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
    return 0;
}