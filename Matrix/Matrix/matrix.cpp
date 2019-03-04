#include "matrix.h"
#include <string>


Matrix::Matrix(const int x, const int y)
{
	x_ = x;
	y_ = y;
	array_ = new int*[x];
	for (auto i = 0; i < x; i++)
	{
		array_[i] = new int[y];
	}
}
Matrix::Matrix(const int x, const int y, const int value)
{
	x_ = x;
	y_ = y;
	array_ = new int*[x];
	for (auto i = 0; i < x; i++)
	{
		array_[i] = new int[y];
		for (auto j = 0; j < y; j++)
		{
			array_[i][j] = value;
		}
	}
}
Matrix::Matrix(Matrix const& m)
{
	copy(m);
}
Matrix::Matrix(Matrix&& m) noexcept
{
	x_ = m.x_;
	y_ = m.y_;
	array_ = m.array_;
}
int* Matrix::operator[](const int i) const
{
	return array_[i];
}
void Matrix::resize(int const x, int const y)
{
	auto **array = new int*[x];
	for (auto i = 0; i < x; i++)
	{
		array[i] = new int[y];
	}
	for (auto i = 0; i < (x_ < x ? x_ : x); i++)
	{
		for (auto j = 0; j < (y_ < y ? y_ : y); j++)
		{
			array[i][j] = array_[i][j];
		}
	}
	x_ = x;
	y_ = y;
	delete array_;
	array_ = array;
}
void Matrix::resize(int const x, int const y, int const value)
{
	auto **array = new int*[x];
	for (auto i = 0; i < x; i++)
	{
		array[i] = new int[y];
		for (auto j = 0; j < y; j++)
		{
			array[i][j] = value;
		}
	}
	for (auto i = 0; i < (x_ < x ? x_ : x); i++)
	{
		for (auto j = 0; j < (y_ < y ? y_ : y); j++)
		{
			array[i][j] = array_[i][j];
		}
	}
	x_ = x;
	y_ = y;
	delete array_;
	array_ = array;
}

Matrix& Matrix::operator=(Matrix const& m)
{
	copy(m);
	return *this;
}

Matrix Matrix::operator+(Matrix const & m) const
{
	if (x_ == m.x_ && y_ == m.y_)
	{
		const auto res = std::make_unique<Matrix>(x_, y_);
		for (auto i = 0; i < x_; i++)
		{
			for (auto j = 0; j < y_; j++)
			{
				(*res)[i][j] = array_[i][j] + m.array_[i][j];
			}
		}
		return *res;
	}
	throw std::invalid_argument("Dimensions must be equal!");
}

Matrix Matrix::operator*(Matrix const& m) const
{
	if (y_ == m.x_)
	{
		const auto res = std::make_unique<Matrix>(x_, m.y_, 0);
		for (auto i = 0; i < x_; i++)
		{
			for (auto j = 0; j < m.y_; j++)
			{
				for (auto a = 0; a < y_; a++)
				{
					(*res)[i][j] += array_[i][a] * m.array_[a][j];
				}
			}
		}
		return *res;
	}
	throw std::invalid_argument("Wrong dimensions!");
}


int Matrix::getDimensions(int const d) const
{
	if (d == 0)
		return x_;
	return y_;
}

int Matrix::max() const
{
	auto a = array_[0][0];
	for (auto i = 0; i < x_; i++)
	{
		for (auto j = 0; j < y_; j++)
		{
			if (a < array_[i][j])
			{
				a = array_[i][j];
			}
		}
	}
	return a;
}
int Matrix::min() const
{
	auto a = array_[0][0];
	for (auto i = 0; i < x_; i++)
	{
		for (auto j = 0; j < y_; j++)
		{
			if (a > array_[i][j])
			{
				a = array_[i][j];
			}
		}
	}
	return a;
}
int Matrix::number_of_entries(const int a) const
{
	auto n = 0;
	for (auto i = 0; i < x_; i++)
	{
		for (auto j = 0; j < y_; j++)
		{
			if (a == array_[i][j])
			{
				n++;
			}
		}
	}
	return n;
}

void Matrix::copy(Matrix const& m)
{
	x_ = m.x_;
	y_ = m.y_;
	array_ = new int*[x_];
	for (auto i = 0; i < x_; i++)
	{
		array_[i] = new int[y_];
		for (auto j = 0; j < y_; j++)
		{
			array_[i][j] = m.array_[i][j];
		}
	}
}


Matrix::~Matrix()
{
	delete array_;
}


std::ostream& operator<<(std::ostream& out, Matrix const & m)
{
	std::string res;
	for (auto i = 0; i < m.x_; i++)
	{
		for (auto j = 0; j < m.y_; j++)
		{
			res += std::to_string(m.array_[i][j]) + '\t';
		}
		res += '\n';
	}
	return out << res;
}
