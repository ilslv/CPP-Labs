#pragma once
#include <vector>
#include <algorithm>
#include <string>

template<typename T>
class kr_array
{
	std::vector<T> array_;
public:
	using iterator = typename std::vector<T>::iterator;
	kr_array() = default;
	kr_array(kr_array const&) = delete;
	kr_array(kr_array&&) = delete;
	virtual ~kr_array() = default;

	iterator begin()
	{
		return array_.begin();
	}

	iterator end()
	{
		return array_.end();
	}

	friend std::ostream& operator<<(std::ostream& out, kr_array const& array)
	{
		for(auto const& e : array.array_)
		{
			out << e;
		}
		return out;
	}

	friend std::istream& operator>>(std::istream& in, kr_array& array)
	{
		std::string line;
		std::getline(in, line);
		array.array_.emplace_back(line);
		return in;
	}

	T& operator[](const int id)
	{
		return array_[id];
	}

	void sort()
	{
		sort(array_.begin(), array_.end());
	}

	iterator find_by_name(std::string const& name)
	{
		return std::find_if(array_.begin(), array_.end(), [&](T const& val) -> bool
		{
			return val.compare_name(name);
		});
	}
};
