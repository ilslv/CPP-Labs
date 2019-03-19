#include "Agency.h"
#include <iostream>
#include <fstream>

int main()
{
	Agency a;
	std::ifstream file("concerts.csv");
	while(!file.eof())
	{
		file >> a;
	}
	a.sort_by_date();
	std::cout << a;
	system("pause");
	return 0;
}
