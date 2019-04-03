#include <iostream>

template<typename T>
class print
{
	void(*bar_)(T value);
public:
	explicit print(void(*foo)(T value))
	{
		bar_ = foo;
	}
	print operator()(const T input) const
	{
		bar_(input);
		return *this;
	}
};

int main()
{
	const print<int> Print([](const int value)
	{
		std::cout << value;
	});
	Print(42)(24)(45)(36);
	system("pause");
	return 0;
}