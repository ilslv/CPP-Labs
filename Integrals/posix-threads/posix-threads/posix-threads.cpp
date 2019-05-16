#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <random>
#include "../../integral_examples.h"
#include <iomanip>

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
	/*std::cout << pthreads_monte_carlo_integral(f1, 0, exp(1), 0, 30, 1000000, 8) << " " <<
				 pthread_simpson_integral(f1, 0, exp(1), 1000000, 8) << " " <<
				 pthreads_gaussian_integral(f1, 0, exp(1), 10000000, 8) << std::endl;*/

	int n = 1000;
	int number_of_threads = 16;

	double res_min, res_avg, res_max,
		time_min, time_avg, time_max,
		err_min, err_avg, err_max,
		val = 14.1542622414793;

	clock_t begin = clock();
	res_min = res_avg = res_max = pthreads_gaussian_integral(f1, 0, exp(1), n, number_of_threads);
	clock_t end = clock();
	time_min = time_avg = time_max = double(end - begin) / CLOCKS_PER_SEC;
	err_min = err_avg = err_max = abs(res_min - val);

	for (auto i = 1; i < 20; ++i)
	{
		begin = clock();
		double cur_res = pthreads_gaussian_integral(f1, 0, exp(1), n, number_of_threads);
		end = clock();

		res_avg += cur_res;
		if (res_min > cur_res)
			res_min = cur_res;
		if (res_max < cur_res)
			res_max = cur_res;

		double cur_time = double(end - begin) / CLOCKS_PER_SEC;
		time_avg += cur_time;
		if (time_min > cur_time)
			time_min = cur_time;
		if (time_max < cur_time)
			time_max = cur_time;

		double cur_err = abs(cur_res - val);
		err_avg += cur_err;
		if (err_min > cur_err)
			err_min = cur_err;
		if (err_max < cur_err)
			err_max = cur_err;

	}
	res_avg /= 20;
	time_avg /= 20;
	err_avg /= 20;
	std::cout << std::setprecision(15) <<
		res_min << "\n" << res_avg << "\n" << res_max << "\n" <<
		time_min << "\n" << time_avg << "\n" << time_max << "\n" <<
		err_min << "\n" << err_avg << "\n" << err_max << "\n";
	return 0;
}
