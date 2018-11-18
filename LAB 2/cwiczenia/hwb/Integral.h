#pragma once
#include <stdio.h>
#include <ctime>
#include <thread>
#include <iostream>

/* Class for calculation integral*/
class Integral
{

	const double x_0 = 2;			//poczatek przedzialu
	const double x_k = 5;			//koniec przedzialu
	const int n = 10000000;

	int ThreadsNumber = 0;

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

	void PrintTime(double TimeSpent)
	{
		std::cout <<"Execution time: "<< TimeSpent << std::endl;
	}

	void PrintIntegralValue(double Result)
	{
		std::cout << "Integral value: " << Result << std::endl;
	}
};

