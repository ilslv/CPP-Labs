#pragma once
#include <stdexcept>

template<typename T>
class matrix
{
	int x_, y_;
	T** m_;
public:
	matrix(const int x, const int y)
	{
		x_ = x;
		y_ = y;
		m_ = new T[x][y];
	}

	matrix(const int x, const int y, const T value)
	{
		x_ = x;
		y_ = y;
		m_ = new T*[x];
		for (auto i = 0; i < x; ++i)
		{
			m_[i] = new T[y]{value};
		}
	}

	~matrix()
	{
		if(m_ != nullptr)
		{
			for (auto i = 0; i < x_; ++i)
			{
				delete[] m_[i];
			}
			delete[] m_;
		}
	}

	T* operator[](const int x)
	{
		throw std::logic_error("Not implemented");
	}

	matrix operator*(const matrix m)
	{
		throw std::logic_error("Not implemented");
	}
};