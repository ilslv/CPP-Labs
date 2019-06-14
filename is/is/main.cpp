#include "is.h"
#include <iostream>

int main()
{
	char s[50] = "";
	is::strcat_m(s, 50, "Hello", "\t", "World", "\t", "!", "\n");
	std::cout << s;

	is::dtoa(s, 50, -3.14, 3);
	std::cout << s << "\n";

	is::array<int> a;
	a.push_back(5);
	a.push_back(3);
	a.push_back(7);
	a.sort();
	a.find(7.f) = 0;
	for(const auto& e : a)
	{
		std::cout << e << " ";
	}
	return 0;
}
