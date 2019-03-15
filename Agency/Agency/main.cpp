#include "Agency.h"
#include <iostream>

int main()
{
	Agency a;
	a.read("concerts.csv");
	a.sort_by_date();
	std::cout << a;
	system("pause");
	return 0;
}
