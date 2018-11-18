#include "Integral.h"
#include <assert.h>
#include <omp.h>



Integral::Integral()
{
	unsigned int nThreads = std::thread::hardware_concurrency();
	ThreadsNumber = nThreads;
}


Integral::~Integral()
{
}

double Integral::Function(double x)
{
	return (x * x * x + 3);
}

void Integral::SequentialOne()
{
	clock_t begin, end;
	double time_spent;

	printf("Obliczanie  calki oznaczonej metoda prostokatow\n");

	begin = clock();
	double calka, x, y = 0;
	double step = (x_k - x_0) / (double)n;
	for (int i = 0; i < n; i++)
	{
		x = x_0 + i * step;
		y = y + Function(x);
	}
	calka = step * y;
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f, czas %f\n", calka, time_spent);
}

void Integral::SequentialTwo()
{
	clock_t begin, end;
	double time_spent;

	printf("Obliczanie  calki oznaczonej metoda prostokatow\n");

	begin = clock();
	double calka, x, y = 0;
	double step = (x_k - x_0) / (double)n;

	for (int i = 0; i < n; i++)
	{
		x = x_0 + (i + 0.5)* step;
		y = y + Function(x);
	}

	calka = step * y;
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f, czas %f\n", calka, time_spent);
}

void Integral::ParallelOne(double& IntegralResult, double& TimeSpent)
{
	clock_t begin, end;

	printf("Calculating integral with using parallel for loop\n");

	begin = clock();
	double x, y = 0;
	double step = (x_k - x_0) / (double)n;

#pragma omp parallel for num_threads (ThreadsNumber) firstprivate (x) reduction(+:y)
	for (int i = 0; i < n; i++)
	{
		x = x_0 + i * step;
		y += Function(x);
	}

	IntegralResult = step * y;
	end = clock();
	TimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

	PrintTime(TimeSpent);
	PrintIntegralValue(IntegralResult);
}

void Integral::ParallelTwo(double& IntegralResult, double& TimeSpent)
{
	clock_t begin, end;

	printf("Calculating integral with using critical sections\n");

	begin = clock();
	double x, y = 0;
	double step = (x_k - x_0) / (double)n;

#pragma omp parallel for num_threads (ThreadsNumber)
	for (int i = 0; i < n; i++)
	{
#pragma omp critical
		{
			x = x_0 + i * step;
			y += Function(x);
		}
	}

	IntegralResult = step * y;
	end = clock();
	TimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

	PrintTime(TimeSpent);
	PrintIntegralValue(IntegralResult);
}

void Integral::ParallelThree(double& IntegralResult, double& TimeSpent)
{
	clock_t begin, end;

	omp_lock_t lock;
	omp_init_lock(&lock);

	printf(" Calculating integral with using locks\n");

	begin = clock();
	double x, y = 0;
	double step = (x_k - x_0) / (double)n;

#pragma omp parallel for num_threads (ThreadsNumber)
	for (int i = 0; i < n; i++)
	{
		if (!omp_test_lock(&lock)) {
			omp_set_lock(&lock);
		}

		x = x_0 + i * step;
		y += Function(x);

		omp_unset_lock(&lock);
	}

	IntegralResult = step * y;
	end = clock();
	TimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	omp_destroy_lock(&lock);

	PrintTime(TimeSpent);
	PrintIntegralValue(IntegralResult);
}

void Integral::ParallelFour(double& IntegralResult, double& TimeSpent)
{
	clock_t begin, end;
	double time_spent;

	printf("Calculating integral with using shedule\n");

	begin = clock();
	double x, y = 0;
	double step = (x_k - x_0) / (double)n;

#pragma omp for schedule (static, ThreadsNumber)
	for (int i = 0; i < n; i++)
	{
		x = x_0 + i * step;
		y += Function(x);
	}

	IntegralResult = step * y;
	end = clock();
	TimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

	PrintTime(TimeSpent);
	PrintIntegralValue(IntegralResult);
}

void Integral::ParallelCpp11(double & IntegralResult, double & TimeSpent)
{
	clock_t begin, end;
	std::vector<std::thread> threadsVector;

	printf("Calculating integral with using threads (cpp11)\n");

	begin = clock();
	int partSteps = n / ThreadsNumber;
	double partRange = ((x_k - x_0) / (double)n) * partSteps;

	ThreadIntegralTask* threadTask = new ThreadIntegralTask(2, 5, partSteps, SumBufor);

	for (int i = 0; i < ThreadsNumber; i++)
	{
		double xBegin = x_0 + (i * partRange);
		double xEnd = xBegin + partRange;
		//std::thread t = std::thread([=] {threadTask->CalculateOnRange(xBegin, xEnd, partSteps); })
		threadsVector.push_back(std::thread([=] {threadTask->CalculateOnRange(xBegin, xEnd, partSteps); }));//std::thread(threadTask.CalculateOnRange, xBegin, xEnd, partSteps));
	}

	for (auto &thread : threadsVector)
	{
		thread.join();
	}

	end = clock();
	TimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

	PrintTime(TimeSpent);
	PrintIntegralValue(threadTask->GetSumBufor());

	delete threadTask;
	threadsVector.clear();
}

void ThreadIntegralTask::CalculateOnRange(double xBegin, double xEnd, int stepsNumber)
{
	double x, y = 0;
	double step = (xEnd - xBegin) / (double)stepsNumber;

	for (int i = 0; i < stepsNumber; i++)
	{
		x = xBegin + i * step;
		y += Function(x);
	}

	y *= step;

	SumBufor = SumBufor + y;

	std::cout << "Sum = " << SumBufor << std::endl;
}

double ThreadIntegralTask::Function(double x)
{
	return (x * x * x + 3);
}
