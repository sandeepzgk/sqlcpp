#include <iostream>
#include "DBManager.h"
#include <time.h>

int main() {

    DBManager dbmanager;
	for (int i = 0; i < 100; i++)
	{
		std::srand(time(0));
		dbmanager.writeMainData(rand(), "P1", "A", rand() % 5, rand() % 5, "[1,2,3,4]", "Y");
		std::srand(time(0));
		dbmanager.writeSRData("P1",rand()%4,rand()%40);
		std::cout<<i+1<<std::endl;
	}

    return 0;
}