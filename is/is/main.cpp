#include "is.h"
#include <iostream>

int main()
{
	char s[50] = "";
	is::strcat_m(s, 50, "test", "\t", "test1", "\t", "test2");
	std::cout << s << "\n";

	is::dtoa(3.14, 2, s, 50);
	std::cout << s << "\n";

	is::array<int> a;
	a.push_back(5);
	a.push_back(3);
	a.push_back(7);
	a.sort();
	a.find(7) = 0;
	for(const auto& e : a)
	{
		std::cout << e << " ";
	}
	return 0;
}
