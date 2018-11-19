#include "ParallelExercise.h"



ParallelExercise::ParallelExercise(int men, int women) : MenPopulation(men), WomenPopulation(women)
{
	srand(time(NULL));

	ThreadsNumber = std::thread::hardware_concurrency();
}


ParallelExercise::~ParallelExercise()
{
}

void ParallelExercise::DoExercise()
{
	omp_lock_t NoWomenInside;
	omp_lock_t NoMenInside;
	omp_init_lock(&NoWomenInside);
	omp_init_lock(&NoMenInside);

	int menInBathroom = 0;
	int womenInBathroom = 0;
	int totalMen = 0;
	int totalWomen = 0;
	std::vector<bool> IsManVectorQueue;

#pragma omp parallel for num_threads (ThreadsNumber) default(shared)
	for (int i = 0; i < MenPopulation + WomenPopulation; i++)
	{
		printf("thread: %d. Male: %s\n", omp_get_thread_num(), (i < MenPopulation ? "Yes" : "No"));

		//#pragma omp critical
		{
			if (i < MenPopulation)
			{
				//Check if man can go inside
				if (womenInBathroom > 0)
				{
					printf("thread: %d. Man can't go inside\n", omp_get_thread_num());
					printf("thread: %d. Set_lock NoMenInside\n", omp_get_thread_num());
					omp_set_lock(&NoWomenInside);
				}

#pragma omp critical
				{
					//Man go inside bathroom
					menInBathroom++;
					totalMen++;
					printf("Man in. Men inside: %d\n", menInBathroom);
				}

				//Rand time inside
				int time = rand() % 100;
				//Sleep(time);

#pragma omp critical
				{
					//Men go outside
					menInBathroom--;
					printf("Man out. Time inside: %d. Men inside: %d\n", time, menInBathroom);
				}

				////If no men - woman can go inside
				if (menInBathroom == 0)
				{
#pragma omp critical
					{
						omp_unset_lock(&NoMenInside);
					}
				}
			}

			if (i >= MenPopulation)
			{

				//Check if woman can go inside
				if (menInBathroom > 0)
				{
					printf("thread: %d. Woman can't go inside\n", omp_get_thread_num());
					printf("thread: %d. Set_lock NoWomenInside\n", omp_get_thread_num());
					omp_set_lock(&NoWomenInside);
				}

#pragma omp critical
				{
					//Man go inside bathroom
					totalWomen++;
					womenInBathroom++;
					printf("Woman in. Women inside: %d\n", womenInBathroom);
				}

				//Rand time inside
				int time = rand() % 100;
				Sleep(time);

#pragma omp critical
				{
					//Men go outside
					womenInBathroom--;
					printf("Woman out. Time inside: %d. Women inside: %d\n", time, womenInBathroom);
				}

				////If no men - woman can go inside
				if (womenInBathroom == 0)
				{
#pragma omp critical
					{
						omp_unset_lock(&NoWomenInside);
					}
				}
			}
		}
	}

	omp_destroy_lock(&NoWomenInside);
	omp_destroy_lock(&NoMenInside);
}
