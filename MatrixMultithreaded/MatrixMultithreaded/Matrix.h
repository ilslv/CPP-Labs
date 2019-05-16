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
		m_ = new T*[x];
		for (auto i = 0; i < x; ++i)
		{
			m_[i] = new T[y];
		}
	}

	matrix(const int x, const int y, const T value)
	{
		x_ = x;
		y_ = y;
		m_ = new T*[x];
		for (auto i = 0; i < x; ++i)
		{
			m_[i] = new T[y];
			for(auto j=0; j<y; ++j)
			{
				m_[i][j] = value;
			}
		}
	}

	matrix(matrix const& m)
	{
		x_ = m.x_;
		y_ = m.y_;
		m_ = new T*[x_];
		for (auto i = 0; i < x_; ++i)
		{
			m_[i] = new T[y_];
			for (auto j = 0; j < y_; ++j)
			{
				m_[i][j] = m.m_[i][j];
			}
		}
	}

	matrix operator=(matrix const& m)
	{
		x_ = m.x_;
		y_ = m.y_;
		m_ = new T*[x_];
		for (auto i = 0; i < x_; ++i)
		{
			m_[i] = new T[y_];
			for (auto j = 0; j < y_; ++j)
			{
				m_[i][j] = m.m_[i][j];
			}
		}
		return *this;
	}

	matrix(matrix&& m) noexcept
	{
		x_ = m.x_;
		y_ = m.y_;
		m_ = m.m_;
		m.m_ = nullptr;
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
		return m_[x];
	}

	matrix operator*(const matrix m)
	{
		if (y_ == m.x_)
		{
			matrix res(x_, m.y_, 0);
			for (auto i = 0; i < x_; i++)
			{
				for (auto j = 0; j < m.y_; j++)
				{
					for (auto a = 0; a < y_; a++)
					{
						res[i][j] += m_[i][a] * m.m_[a][j];
					}
				}
			}
			return res;
		}
		throw std::invalid_argument("Wrong dimensions!");
	}
};