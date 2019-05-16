#pragma once
#include <stdexcept>

template<typename T>
class matrix
{
	int x_, y_;
	T** m_;
public:
	matrix(const int x, const int y);
	matrix(const int x, const int y, const T value);
	~matrix();

	T* operator[](const int x)
	{
		throw std::logic_error("Not implemented");
	}

	matrix operator*(const matrix m)
	{
		throw std::logic_error("Not implemented");
	}
};