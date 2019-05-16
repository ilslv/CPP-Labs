#include <iostream>
#include <random>
#include "../../integral_examples.h"
#include <iomanip>
#include <ctime>

double monte_carlo_integral(double(*f)(double), const double x_min, const double x_max, const double y_min,
	const double y_max, const int n)
{
	auto in_box = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	const std::uniform_real_distribution<> dis(0.0, 1.0);
	for (auto i = 0; i < n; i++)
	{
		const auto x = ((x_max - x_min) * dis(gen)) + x_min;
		const auto y = ((y_max - y_min) * dis(gen)) + y_min;
		const auto val = f(x);
		if (val > y)
		{
			in_box++;
		}
	}
	const auto density = 1. * in_box / n;
	return (x_max - x_min) * (y_max - y_min) * density;
}

double simpson_integral(double(*f)(double), const double x_from, const double x_to, const int n)
{
	double start = x_from, result = 0;
	const auto delta = (x_to - x_from) / n;
	for (auto i = 0; i < n; i++)
	{
		result += delta / 6 * (f(start) + 4 * f(start + delta / 2) + f(start + delta));
		start += delta;
	}
	return result;
}

double gaussian_integral(double(*f)(double), const double x_from, const double x_to, const int n)
{
	const auto x1 = (sqrt(5. - 2. * sqrt(10. / 7))) / 3.;
	const auto x2 = (sqrt(5. + 2. * sqrt(10. / 7))) / 3.;
	const auto w1 = 128. / 225;
	const auto w2 = (322. + 13. * sqrt(70.)) / 900.;
	const auto w3 = (322. - 13. * sqrt(70.)) / 900.;
	const double x[]{ 0, x1, -1. * x1, x2, -1. * x2 };
	const double w[]{ w1, w2, w2, w3, w3 };

	double start = x_from, result = 0;
	const auto delta = (x_to - x_from) / n;
	for (auto i = 0; i < n; i++)
	{
		double current_result = 0;
		for (auto j = 0; j < 5; j++)
		{
			current_result += w[j] * f(delta / 2. * x[j] + start + delta / 2.);
		}
		current_result *= delta / 2.;
		result += current_result;
		start += delta;
	}
	return result;
}

int main()
{
	//int n = 100000000;
	//clock_t begin = clock();
	//auto result = monte_carlo_integral(f1, 0, exp(1), 0, 30, n);
	//clock_t end = clock();
	//std::cout << std::setprecision(20) << result << " " << double(end - begin) / CLOCKS_PER_SEC << std::endl;
	//begin = clock();
	//result = simpson_integral(f1, 0, exp(1), n);
	//end = clock();
	//std::cout << std::setprecision(20) << result << " " << double(end - begin) / CLOCKS_PER_SEC << std::endl;
	//begin = clock();
	//result = gaussian_integral(f1, 0, exp(1), n);
	//end = clock();
	//std::cout << std::setprecision(20) << result << " " << double(end - begin) / CLOCKS_PER_SEC << std::endl;

	int n = 100000;

	double res_min, res_avg, res_max, 
	time_min, time_avg, time_max, 
	err_min, err_avg, err_max, 
	val = 14.1542622414793;
	
	clock_t begin = clock();
	res_min = res_avg = res_max = simpson_integral(f1, 0, exp(1), n);
	clock_t end = clock();
	time_min = time_avg = time_max = double(end - begin) / CLOCKS_PER_SEC;
	err_min = err_avg = err_max = abs(res_min - val);
	std::cout << std::setprecision(15) <<
		res_min << "\n" << res_avg << "\n" << res_max << "\n" <<
		time_min << "\n" << time_avg << "\n" << time_max << "\n" <<
		err_min << "\n" << err_avg << "\n" << err_max << "\n";
	return 0;
}
