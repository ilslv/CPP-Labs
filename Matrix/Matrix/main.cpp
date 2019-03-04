#include "matrix.h"

int main()
{
	Matrix m(3, 3, 1);
	m.resize(5, 5, 1);
	m[0][0] = 10;
	m = m + m;
	m = m * m;
	std::cout << m;
	system("pause");
	return 0;
}