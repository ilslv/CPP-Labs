#pragma once
#include <stdexcept>
#include <vector>
#include <thread>
#include <atomic>

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
		const auto number_of_threads = 8;
		if (y_ == m.x_)
		{
			matrix res(x_, m.y_, 0);

			std::vector<std::thread> threads;

			threads.reserve(number_of_threads);

			for (auto thread_num = 0; thread_num < number_of_threads; thread_num++)
			{
				threads.emplace_back(std::thread([&, thread_num]()
				{
					const int number_of_cells = ceil(1. * res.x_ * res.y_ / number_of_threads);
					for(auto i = thread_num * number_of_cells; 
						i < res.x_ * res.y_ && i < (thread_num + 1) * number_of_cells; 
						i++
						)
					{
						int x = i / res.x_, y = i % res.y_, result = 0;
						for(auto j = 0; j < y_; j++)
						{
							result += m_[x][j] * m.m_[j][y];
						}
						res[x][y] = result;
					}
				}));
			}

			for (auto& t: threads)
			{
				t.join();
			}

			return res;
		}
		throw std::invalid_argument("Wrong dimensions!");
	}
};
