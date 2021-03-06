// Cpp11_OR_DR.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <time.h> 
#include <vector>

//#include "Integral.h"
#include "BathroomExercise.h"

void Print(std::string Message)
{
	std::cout << Message.c_str() << std::endl;
}

int main()
{
	//double a, b;
	//Integral integral;
	//integral.SequentialOne();
	//integral.ParallelOne(a, b);
	//integral.ParallelTwo(a, b);
	//integral.ParallelThree(a, b);
	//integral.ParallelFour(a, b);
	//integral.ParallelCpp11(a,b);

	Bathroom* bathroom = new Bathroom();
	std::vector<Person*> persons;
	std::vector<std::thread> threads;

	srand(time(NULL));

	for (int i = 0; i < 30; i++)
	{
		int r = rand() % 2;

		if (r)
		{
			persons.push_back(new Man(bathroom));
			Print("Added man");
		}
		else
		{
			persons.push_back(new Woman(bathroom));
			Print("Added woman");
		}
	}

	for (auto person : persons)
	{
		threads.push_back(std::thread([=] {person->UseBathroom(); }));
	}

	for (auto &thread : threads)
	{
		thread.join();
	}


	persons.clear();
	delete bathroom;

	system("pause");

    return 0;
}

