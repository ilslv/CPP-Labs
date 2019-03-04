#include <iostream>
#include <string>

class number_sequence
{
	int* array_;
	int count_;
	int size_;

	friend std::ostream& operator<<(std::ostream& out, const number_sequence& obj);
public:
	explicit number_sequence(int size) noexcept
	{
		array_ = static_cast<int *>(malloc(size * sizeof(size)));
		this->size_ = size;
		count_ = 0;
	}

	number_sequence(number_sequence const& source)
	{
		size_ = source.size_;
		count_ = source.count_;
		array_ = static_cast<int *>(malloc(size_ * sizeof(size_)));
		for (auto i = 0; i < count_; i++)
		{
			array_[i] = source.array_[i];
		}
	}

	number_sequence(number_sequence&& source) noexcept
	{
		size_ = source.size_;
		count_ = source.count_;
		array_ = source.array_;
	}

	void append(int const value)
	{
		if (count_ == size_)
		{
			array_ = static_cast<int *>(realloc(array_, size_ * 2 * sizeof(size_)));
			size_ *= 2;
		}
		array_[count_] = value;
		count_++;
	}

	int sum() const
	{
		auto sum = 0;
		for (auto i = 0; i < count_; i++)
		{
			sum += array_[i];
		}
		return sum;
	}

	int min() const
	{
		auto res = array_[0];
		for (auto i = 1; i < count_; i++)
		{
			if (array_[i] < res)
			{
				res = array_[i];
			}
		}
		return res;
	}

	int max() const
	{
		auto res = array_[0];
		for (auto i = 1; i < count_; i++)
		{
			if (array_[i] > res)
			{
				res = array_[i];
			}
		}
		return res;
	}

	double mean() const
	{
		return static_cast<double>(sum()) / count_;
	}

	virtual ~number_sequence()
	{
		free(array_);
	}
};

std::ostream& operator<<(std::ostream& out, const number_sequence& obj)
{
	std::string res;
	for (auto i = 0; i < obj.count_; i++)
	{
		res += std::to_string(obj.array_[i]) + " ";
	}
	return out << res;
}

std::istream& operator>>(std::istream& in, number_sequence& obj)
{
	std::string s;
	in >> s;
	try
	{
		obj.append(stoi(s));
	}
	catch (const std::invalid_argument&)
	{
		throw std::invalid_argument("Error: Failed to transform to int!");
	}
	return in;
}

int main()
{
	number_sequence array(5);
	std::cout << "Enter a non-int to stop program\n";
	while (true)
	{
		try
		{
			std::cin >> array;
		}
		catch (const std::invalid_argument&)
		{
			break;
		}
	}

	std::cout << array << "\nSum = " <<
		array.sum() << "\nMean = " << array.mean()
		<< "\nMax = " << array.max() << "\nMin = " <<
		array.min() << std::endl;
	system("pause");
	return 0;
}
