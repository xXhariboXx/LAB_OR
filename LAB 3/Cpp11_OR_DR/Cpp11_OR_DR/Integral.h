#pragma once
#include <stdio.h>
#include <ctime>
#include <thread>
#include <iostream>
#include <vector>
#include <atomic>


class ThreadIntegralTask
{

private:
	double x_0 = 2;			//poczatek przedzialu
	double x_k = 5;			//koniec przedzialu
	const int n = 10000000;
	std::atomic <double> SumBufor;

public:

	ThreadIntegralTask(double xBegin, double xEnd, int steps, double& bufor) : x_0(xBegin), x_k(xEnd), n(steps), SumBufor(bufor) {}

	double GetSumBufor() { return SumBufor; }

	void CalculateOnRange(double xBegin, double xEnd, int stepsNumber);

private:
	double Function(double x);
};

/* Class for calculation integral*/
class Integral
{

	const double x_0 = 2;			//poczatek przedzialu
	const double x_k = 5;			//koniec przedzialu
	const int n = 10000000;

	int ThreadsNumber = 0;

	double SumBufor = 0;

private:
	double Function(double x);

public:
	Integral();
	~Integral();

	void SequentialOne();
	void SequentialTwo();

	/* Calculating integral with using parallel for loop */
	void ParallelOne(double& IntegralResult, double& TimeSpend);
	/* Calculating integral with using critical sections */
	void ParallelTwo(double& IntegralResult, double& TimeSpend);
	/* Calculating integral with using locks */
	void ParallelThree(double& IntegralResult, double& TimeSpend);
	/* Calculating integral with using shedule */
	void ParallelFour(double& IntegralResult, double& TimeSpend);

	void ParallelCpp11(double& IntegralResult, double& TimeSpend);

	void PrintTime(double TimeSpent)
	{
		std::cout << "Execution time: " << TimeSpent << std::endl;
	}

	void PrintIntegralValue(double Result)
	{
		std::cout << "Integral value: " << Result << std::endl;
	}
};

