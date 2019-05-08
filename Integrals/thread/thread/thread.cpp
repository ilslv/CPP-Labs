#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <random>
#include <cmath>
#include "../../integral_examples.h"

double thread_monte_carlo_integral(double (*f)(double), const double x_min, const double x_max, const double y_min,
                                   const double y_max, const int n, const int number_of_threads)
{
	std::atomic<int> total_in_box(0);
	std::vector<std::thread> threads;
	threads.reserve(number_of_threads);
	const int number_of_points = ceil(1. * n / number_of_threads);
	for (auto i = 0; i < number_of_threads; i++)
	{
		threads.emplace_back(std::thread([&]()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			const std::uniform_real_distribution<> dis(0.0, 1.0);
			for (auto j = 0; j < number_of_points; j++)
			{
				const auto x = ((x_max - x_min) * dis(gen)) + x_min;
				const auto y = ((y_max - y_min) * dis(gen)) + y_min;
				const auto val = f(x);
				if (val > y)
				{
					++total_in_box;
				}
			}
		}));
	}
	for (auto i = 0; i < number_of_threads; i++)
	{
		threads[i].join();
	}
	return (x_max - x_min) * (y_max - y_min) * total_in_box / (ceil(1. * n / number_of_threads) * number_of_threads);
}

double thread_simpsons_integral(double(*f)(double), const double x_from, const double x_to, const int n, const int number_of_threads)
{
	std::atomic<double> result(0);
	std::vector<std::thread> threads;
	threads.reserve(number_of_threads);
	const auto delta = (x_to - x_from) / n;
	const int number_of_iterations = ceil(1. * n / number_of_threads);
	for(auto i = 0; i < number_of_threads; i++)
	{
		threads.emplace_back(std::thread([&]()
		{
			auto start = x_from + delta * number_of_iterations * i;
			double thread_result = 0;
			for (auto j = 0; j < number_of_iterations; j++)
			{
				thread_result += delta / 6 * (f(start) + 4 * f(start + delta / 2) + f(start + delta));
				start += delta;
			}
			result = result + thread_result;
		}));
	}
	for (auto i = 0; i < number_of_threads; i++)
	{
		threads[i].join();
	}
	return result;
}

double thread_gaussian_integral(double(*f)(double), const double x_from, const double x_to, const int n, const int number_of_threads)
{
	const auto x1 = (sqrt(5. - 2. * sqrt(10. / 7))) / 3.;
	const auto x2 = (sqrt(5. + 2. * sqrt(10. / 7))) / 3.;
	const auto w1 = 128. / 225;
	const auto w2 = (322. + 13. * sqrt(70.)) / 900.;
	const auto w3 = (322. - 13. * sqrt(70.)) / 900.;
	const double x[]{ 0, x1, -1. * x1, x2, -1. * x2 };
	const double w[]{ w1, w2, w2, w3, w3 };

	std::atomic<double> result(0);
	std::vector<std::thread> threads;
	threads.reserve(number_of_threads);

	const auto delta((x_to - x_from) / n);
	const auto number_of_iterations(ceil(1. * n / number_of_threads));
	for (auto i = 0; i < number_of_threads; i++)
	{
		threads.emplace_back(std::thread([&]()
		{
			auto start = x_from + delta * number_of_iterations * i;
			double thread_result = 0;

			for (auto k = 0; k < number_of_iterations; k++)
			{
				double current_result = 0;
				for (auto j = 0; j < 5; j++)
				{
					current_result += w[j] * f(delta / 2. * x[j] + start + delta / 2.);
				}
				current_result *= delta / 2.;
				thread_result += current_result;
				start += delta;
			}

			result = result + thread_result;
		}));
	}
	for (auto i = 0; i < number_of_threads; i++)
	{
		threads[i].join();
	}
	return result;
}

int main()
{
	const auto number_of_threads = 8;
	std::cout << thread_monte_carlo_integral(f1, 0, exp(1), 0, 30, 1000000, number_of_threads) << " " <<
				 thread_simpsons_integral(f1, 0, exp(1), 1000000, number_of_threads) << " " <<
				 thread_gaussian_integral(f1, 0, exp(1), 1000000, number_of_threads);
	return 0;
}
