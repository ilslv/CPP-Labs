#include <mpi.h>
#include <cmath>
#include <random>
#include <iostream>
#include "../../integral_examples.h"
#include <iomanip>

double mpi_monte_carlo_integral(double (*f)(double), const double x_min, const double x_max, const double y_min,
                              const double y_max, const int n)
{
	int rank, size, in_box = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	const std::uniform_real_distribution<> dis(0.0, 1.0);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	const int number_of_points = ceil(1. * n / size);
	for (auto i = 0; i < number_of_points; i++)
	{
		const auto x = ((x_max - x_min) * dis(gen)) + x_min;
		const auto y = ((y_max - y_min) * dis(gen)) + y_min;
		const auto val = f(x);
		if (val > y)
		{
			in_box++;
		}
	}
	int resulting_in_box;
	MPI_Reduce(&in_box, &resulting_in_box, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	return resulting_in_box / (ceil(1. * n / size) * size) * (x_max - x_min) * (y_max - y_min);
}

double mpi_simpsons_integral(double (*f)(double), const double x_from, const double x_to, const int n)
{
	int rank, size;
	double result;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	const auto delta = (x_to - x_from) / n;
	const int number_of_iterations = ceil(1. * n / size);
	auto start = x_from + delta * number_of_iterations * rank;
	double thread_result = 0;
	for (auto i = 0; i < number_of_iterations; i++)
	{
		thread_result += delta / 6 * (f(start) + 4 * f(start + delta / 2) + f(start + delta));
		start += delta;
	}
	MPI_Reduce(&thread_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	return result;
}

double mpi_gaussian_integral(double (*f)(double), const double x_from, const double x_to, const int n)
{
	int rank, size;
	double result;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const auto x1 = (sqrt(5. - 2. * sqrt(10. / 7))) / 3.;
	const auto x2 = (sqrt(5. + 2. * sqrt(10. / 7))) / 3.;
	const auto w1 = 128. / 225;
	const auto w2 = (322. + 13. * sqrt(70.)) / 900.;
	const auto w3 = (322. - 13. * sqrt(70.)) / 900.;
	const double x[]{0, x1, -1. * x1, x2, -1. * x2};
	const double w[]{w1, w2, w2, w3, w3};

	const auto delta = (x_to - x_from) / n;
	const int number_of_iterations = ceil(1. * n / size);
	auto start = x_from + delta * number_of_iterations * rank;
	double thread_result = 0;

	for (auto i = 0; i < number_of_iterations; i++)
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
	MPI_Reduce(&thread_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	return result;
}

int main()
{
	//MPI_Init(nullptr, nullptr);
	//int rank;
	//MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//double result;
	//auto start_time = MPI_Wtime();
	//mpi_monte_carlo_integral(f1, 0, exp(1), 0, 30, 100000000, result);
	//auto end_time = MPI_Wtime();
	//if (rank == 0)
	//{
	//	std::cout << result << " " << end_time - start_time << std::endl;
	//}
	//start_time = MPI_Wtime();
	//mpi_simpsons_integral(f1, 0, exp(1), 100000000, result);
	//end_time = MPI_Wtime();
	//if (rank == 0)
	//{
	//	std::cout << result << " " << end_time - start_time << std::endl;
	//}
	//start_time = MPI_Wtime();
	//mpi_gaussian_integral(f1, 0, exp(1), 100000000, result);
	//end_time = MPI_Wtime();
	//if (rank == 0)
	//{
	//	std::cout << result << " " << end_time - start_time << std::endl;
	//}
	//MPI_Finalize();

	MPI_Init(nullptr, nullptr);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n = 100000000;

	double res_min, res_avg, res_max,
		time_min, time_avg, time_max,
		err_min, err_avg, err_max,
		val = 14.1542622414793;

	auto begin = MPI_Wtime();
	res_min = res_avg = res_max = mpi_monte_carlo_integral(f1, 0, exp(1), 0, 20, n);
	auto end = MPI_Wtime();
	time_min = time_avg = time_max = end - begin;
	err_min = err_avg = err_max = abs(res_min - val);

	for (auto i = 1; i < 20; ++i)
	{
		begin = MPI_Wtime();
		double cur_res = mpi_monte_carlo_integral(f1, 0, exp(1), 0, 20, n);
		end = MPI_Wtime();
		
		if(rank == 0)
		{
			res_avg += cur_res;
			if (res_min > cur_res)
				res_min = cur_res;
			if (res_max < cur_res)
				res_max = cur_res;

			double cur_time = end - begin;
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
	}
	res_avg /= 20;
	time_avg /= 20;
	err_avg /= 20;
	if(rank == 0)
	{
		std::cout << std::setprecision(15) <<
			res_min << "\n" << res_avg << "\n" << res_max << "\n" <<
			time_min << "\n" << time_avg << "\n" << time_max << "\n" <<
			err_min << "\n" << err_avg << "\n" << err_max << "\n";
	}
	MPI_Finalize();
	return 0;
}
