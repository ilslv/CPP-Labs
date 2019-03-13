#include "Agency.h"
#include <iostream>
#include <Windows.h>

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Agency a;
	a.read("concerts.csv");
	a.sort_by_date();
	std::cout << a;
	system("pause");
	return 0;
}
