#include <iostream>

class print
{
public:
	print operator()(const int input) const
	{
		std::cout << input;
		return *this;
	}
};

int main()
{
	print Print;
	Print(42)(24)(45)(36);
	system("pause");
	return 0;
}