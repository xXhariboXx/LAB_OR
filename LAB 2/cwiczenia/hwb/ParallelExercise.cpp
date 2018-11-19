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
	std::deque<bool> IsManVectorQueue;

#pragma omp parallel for num_threads (ThreadsNumber) default(shared)
	for (int i = 0; i < MenPopulation + WomenPopulation; i++)
	{
		printf("thread: %d. Male: %s\n", omp_get_thread_num(), (i < MenPopulation ? "Yes" : "No"));

#pragma omp critical
		IsManVectorQueue.push_back((i < MenPopulation) ? true : false);

		//#pragma omp critical
		{
			if (i < MenPopulation)
			{
				//Check if man can go inside
				while (womenInBathroom >= 0)
				{
					if (womenInBathroom == 0)
					{
						if (IsManVectorQueue.size() > 0 && IsManVectorQueue[0])
						{
#pragma omp critical
							IsManVectorQueue.pop_front();
							break;
						}
						else if (IsManVectorQueue.size() == 0)
						{
							break;
						}
					}
				}


				//Man go inside bathroom
#pragma omp critical
				{
					menInBathroom++;
					totalMen++;
					printf("thread: %d. Man in. Men inside: %d. Women inside: %d\n", omp_get_thread_num(), menInBathroom, womenInBathroom);
				}

				//Rand time inside
				int time = rand() % 100;
				Sleep(time);

#pragma omp critical
				{
					//Men go outside
					menInBathroom--;
					printf("thread: %d. Man out. Time inside: %d. Men inside: %d\n", omp_get_thread_num(), time, menInBathroom);
				}
			}

			if (i >= MenPopulation)
			{

				//Check if woman can go inside
				while (menInBathroom >= 0)
				{
					if (menInBathroom == 0)
					{
						if (IsManVectorQueue.size() > 0 && !IsManVectorQueue[0])
						{
#pragma omp critical
							IsManVectorQueue.pop_front();
							break;
						}
						else if (IsManVectorQueue.size() == 0)
						{
							break;
						}
					}
				}

#pragma omp critical
				{
					//Man go inside bathroom
					womenInBathroom++;
					totalWomen++;
					printf("thread: %d. Woman in. Women inside: %d. Men inside: %d\n", omp_get_thread_num(), womenInBathroom, menInBathroom);
				}

				//Rand time inside
				int time = rand() % 100;
				Sleep(time);

#pragma omp critical
				{
					//Men go outside
					womenInBathroom--;
					printf("thread: %d. Woman out. Time inside: %d. Women inside: %d\n", omp_get_thread_num(), time, womenInBathroom);
				}
			}
		}
	}

	omp_destroy_lock(&NoWomenInside);
	omp_destroy_lock(&NoMenInside);
}
