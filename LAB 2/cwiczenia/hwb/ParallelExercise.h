#pragma once

#include <thread>
#include <omp.h>
#include <stdlib.h>
#include <time.h>   
#include <windows.h>
#include <deque>

/* Class for executing parallel tasks (exercise number 2)*/
class ParallelExercise
{
	int ThreadsNumber = 0;

	int MenPopulation, WomenPopulation;

public:

	ParallelExercise(int men, int women);
	~ParallelExercise();

	void DoExercise();

};

