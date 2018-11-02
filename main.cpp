#include <iostream>
#include "DBManager.h"
#include <time.h>
#include <thread>

int main() {

    DBManager *dbmanager = DBManager::GetInstance();
	for (int i = 0; i < 100; i++)
	{
		std::srand(time(0));
		dbmanager->writeMainData(rand(), "P1", "A", rand() % 5, rand() % 5, "[1,2,3,4]", "Y");
		std::srand(time(0));
		dbmanager->writeSRData("P1",rand()%4,rand()%40);
		std::cout<<i+1<<std::endl;
	}
	while(dbmanager->isBusy())
	{
		std::cout<<"DBManager is busy"<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
    return 0;
}