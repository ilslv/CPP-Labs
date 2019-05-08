#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <random>
#include "../../integral_examples.h"

double pthreads_monte_carlo_integral(double (*f)(double), const double x_min, const double x_max, const double y_min,
                                     const double y_max, const int n, const int number_of_threads)
{
	struct integral_attributes
	{
		double (*f)(double);
		const double *x_min, *x_max, *y_min, *y_max;
		const int *number_of_points;
	};
	const auto number_of_points = static_cast<int>(ceil(1. * n / number_of_threads));
	const auto info = new integral_attributes{
		f, &x_min, &x_max, &y_min, &y_max, &number_of_points
	};
	const auto threads = new pthread_t[number_of_threads];
	auto total_in_box = 0;
	for (auto i = 0; i < number_of_threads; i++)
	{
		pthread_create(&threads[i], nullptr, [](void* info) -> void*
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			const std::uniform_real_distribution<> dis(0.0, 1.0);
			auto in_box = 0;
			for (auto j = 0; j < *(static_cast<integral_attributes*>(info)->number_of_points); j++)
			{
				const auto x = ((*(static_cast<integral_attributes*>(info)->x_max) - *(static_cast<integral_attributes*>
					(info)->x_min)) * dis(gen)) + *(static_cast<integral_attributes*>(info)->x_min);
				const auto y = ((*(static_cast<integral_attributes*>(info)->y_max) - *(static_cast<integral_attributes*>
					(info)->y_min)) * dis(gen)) + *(static_cast<integral_attributes*>(info)->y_min);
				const auto val = (static_cast<integral_attributes*>(info)->f)(x);
				if (val > y)
				{
					++in_box;
				}
			}

			return reinterpret_cast<void*>(in_box);
		}, static_cast<void *>(info));
	}
	void* res;
	for (auto i = 0; i < number_of_threads; i++)
	{
		pthread_join(threads[i], &res);
		total_in_box += reinterpret_cast<int>(res);
	}
	delete info;
	return (x_max - x_min) * (y_max - y_min) * total_in_box / (ceil(1. * n / number_of_threads) * number_of_threads);
}

double pthread_simpson_integral(double (*f)(double), const double x_from, const double x_to, const int n,
                                const int number_of_threads)
{
	struct integral_attributes
	{
		double (*f)(double);
		const double* delta;
		double start;
		const int* number_of_iterations;
	};
	double result = 0;
	const auto delta = (x_to - x_from) / n;
	const auto number_of_iterations = static_cast<int>(ceil(1. * n / number_of_threads));
	const auto threads = new pthread_t[number_of_threads];
	for (auto i = 0; i < number_of_threads; ++i)
	{
		const auto info = new integral_attributes{
			f, &delta, (x_from + delta * number_of_iterations * i), &number_of_iterations
		};
		pthread_create(&threads[i], nullptr, [](void* info) -> void*
		{
			const auto thread_result = new double(0);
			for (auto j = 0; j < *((static_cast<integral_attributes*>(info))->number_of_iterations); j++)
			{
				*thread_result += *((static_cast<integral_attributes*>(info))->delta) / 6 * (((static_cast<
					integral_attributes*>(info))->f)((static_cast<integral_attributes*>(info))->start) + 4 * ((
					static_cast<integral_attributes*>(info))->f)(
					((static_cast<integral_attributes*>(info))->start) + *((static_cast<integral_attributes*>(info))->
						delta) / 2) + ((static_cast<integral_attributes*>(info))->f)(
					((static_cast<integral_attributes*>(info))->start) + *((static_cast<integral_attributes*>(info))->
						delta)));
				((static_cast<integral_attributes*>(info))->start) += *((static_cast<integral_attributes*>(info))->delta
				);
			}
			return reinterpret_cast<void*>(thread_result);
		}, static_cast<void *>(info));
	}
	void* res = nullptr;
	for (auto i = 0; i < number_of_threads; ++i)
	{
		pthread_join(threads[i], &res);
		result += *(static_cast<double*>(res));
	}
	delete static_cast<double*>(res);
	return result;
}

double pthreads_gaussian_integral(double (*f)(double), const double x_from, const double x_to, const int n,
                                  const int number_of_threads)
{
	struct integral_attributes
	{
		double(*f)(double);
		const double *w, *x, *delta;
		double start;
		const int *number_of_iterations;
	};

	const auto threads = new pthread_t[number_of_threads];

	double result = 0;

	const auto x1 = (sqrt(5. - 2. * sqrt(10. / 7))) / 3.;
	const auto x2 = (sqrt(5. + 2. * sqrt(10. / 7))) / 3.;
	const auto w1 = 128. / 225;
	const auto w2 = (322. + 13. * sqrt(70.)) / 900.;
	const auto w3 = (322. - 13. * sqrt(70.)) / 900.;
	const double x[]{ 0, x1, -1. * x1, x2, -1. * x2 };
	const double w[]{ w1, w2, w2, w3, w3 };

	const auto number_of_iterations = static_cast<int>(ceil(1. * n / number_of_threads));
	const auto delta = (x_to - x_from) / n;
	for (auto i = 0; i < number_of_threads; ++i)
	{
		const auto info = new integral_attributes{ f, w, x, &delta, (x_from + delta * number_of_iterations * i), &number_of_iterations };
		pthread_create(&threads[i], nullptr, [](void *info) -> void*
		{
			const auto thread_result = new double(0);
			
			for (auto k = 0; k < *(static_cast<integral_attributes*>(info)->number_of_iterations); k++)
			{
				double current_result = 0;
				for (auto j = 0; j < 5; j++)
				{
					current_result += (static_cast<integral_attributes*>(info)->w)[j] * (static_cast<integral_attributes*>(info)->f)(*(static_cast<integral_attributes*>(info)->delta) / 2. * (static_cast<integral_attributes*>(info)->x)[j] + (static_cast<integral_attributes*>(info)->start) + *(static_cast<integral_attributes*>(info)->delta) / 2.);
				}
				current_result *= *(static_cast<integral_attributes*>(info)->delta) / 2.;
				*thread_result += current_result;
				(static_cast<integral_attributes*>(info)->start) += *(static_cast<integral_attributes*>(info)->delta);
			}

			return static_cast<void*>(thread_result);
		}, static_cast<void*>(info));
	}
	void *res = nullptr;
	for (auto i = 0; i < number_of_threads; ++i)
	{
		pthread_join(threads[i], &res);
		result += *(static_cast<double*>(res));
	}
	delete static_cast<double*>(res);
	return result;
}

int main()
{
	std::cout << pthreads_monte_carlo_integral(f1, 0, exp(1), 0, 30, 1000000, 8) << " " <<
				 pthread_simpson_integral(f1, 0, exp(1), 1000000, 8) << " " <<
				 pthreads_gaussian_integral(f1, 0, exp(1), 10000000, 8) << std::endl;
	return 0;
}
