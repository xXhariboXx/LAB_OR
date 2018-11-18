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
	omp_lock_t LockMen;
	omp_lock_t LockWomen;
	omp_init_lock(&LockMen);
	omp_init_lock(&LockWomen);
	int menInBathroom = 0;
	int womenInBathroom = 0;
	int totalMen = 0;
	int totalWomen = 0;

#pragma omp parallel for num_threads (ThreadsNumber) shared(menInBathroom, womenInBathroom, LockMen, LockWomen)
	for (int i = 0; i < MenPopulation + WomenPopulation; i++)
	{
		printf("thread: %d\n", omp_get_thread_num());

		if (i < MenPopulation)
		{	
			//Check if man can go inside
			while (/*!omp_test_lock(&LockMen) ||*/womenInBathroom > 0)
			{
			}

			/*omp_set_lock(&LockWomen);*/

			//Man go inside bathroom
			menInBathroom++;
			totalMen++;
			printf("Man in. Men inside: %d\n", menInBathroom);

			//Rand time inside
			int time = rand() % 100;
			Sleep(time);

			//Men go outside
			menInBathroom--;
			printf("Man out. Time inside: %d. Men inside: %d\n", time, menInBathroom);

			////If no men - woman can go inside
			//if (menInBathroom == 0)
			//{
			//	omp_unset_lock(&LockWomen);
			//}
		}

		if (i >= MenPopulation)
		{
			//Check if woman can go inside
			while (/*!omp_test_lock(&LockWomen) || */menInBathroom > 0)
			{
			}

			/*omp_set_lock(&LockMen);*/

			//Man go inside bathroom
			totalWomen++;
			womenInBathroom++;
			printf("Woman in. Women inside: %d\n", womenInBathroom);

			//Rand time inside
			int time = rand() % 100;
			Sleep(time);

			//Men go outside
			womenInBathroom--;
			printf("Woman out. Time inside: %d. Women inside: %d\n", time, womenInBathroom);

			////If no men - woman can go inside
			//if (womenInBathroom == 0)
			//{
			//	omp_unset_lock(&LockMen);
			//}
		}
	}

	omp_destroy_lock(&LockMen);
	omp_destroy_lock(&LockWomen);
}
