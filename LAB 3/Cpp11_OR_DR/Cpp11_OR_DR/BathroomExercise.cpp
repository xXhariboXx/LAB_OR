#include "BathroomExercise.h"



BathroomExercise::BathroomExercise()
{
}


BathroomExercise::~BathroomExercise()
{
}

void Man::UseBathroom()
{
	BathroomPtr->IncrementMen();

	Sleep(100);

	BathroomPtr->DecrementMen();
}

void Woman::UseBathroom()
{
	BathroomPtr->IncrementWomen();

	Sleep(100);

	BathroomPtr->DecrementWomen();
}

EGender Bathroom::GetFirstGenderInQueue()
{
	EGender firstGenderInQueue = EGender::None;

	if (PeopleQueue.size() > 0)
	{
		firstGenderInQueue = PeopleQueue[0];
	}

	return firstGenderInQueue;
}

Bathroom::Bathroom()
{
}

Bathroom::~Bathroom()
{
}

void Bathroom::IncrementMen()
{
	std::unique_lock<std::mutex> lock(BathroomQueueMutex);

	if (GenderInside == EGender::Female || GetFirstGenderInQueue() == EGender::Female)
	{
		PrintError("Men want to go inside bathroom while women inside");

		PeopleQueue.push_back(EGender::Male);

		NoWomenInside.wait(lock);
	}

	GenderInside = (GenderInside == EGender::None )? EGender::Male : GenderInside;

	NumMen++;
	PrintMessage("Man walked in");

	PrintMenInside();
}

void Bathroom::DecrementMen()
{
	BathroomQueueMutex.lock();
	NumMen--;

	if (NumMen == 0)
	{
		GenderInside = EGender::None;
		int counter = 0;
		EGender firstGenderInQueue = GetFirstGenderInQueue();

		while (counter < PeopleQueue.size() && firstGenderInQueue == PeopleQueue[counter])
		{
			counter++;
		}

		for (int i = 0; i < counter; i++)
		{
			if (!PeopleQueue.empty())
			{
				PeopleQueue.pop_front();
			}

			NoMenInside.notify_one();
		}
	}
	else if (NumMen < 0)
	{
		PrintError("Less than 0 men");
	}

	PrintMessage("Man walked out");
	PrintMenInside();

	BathroomQueueMutex.unlock();
}

void Bathroom::IncrementWomen()
{
	std::unique_lock<std::mutex> lock(BathroomQueueMutex);

	if (GenderInside == EGender::Male || GetFirstGenderInQueue() == EGender::Male)
	{
		PrintError("Women want to go inside bathroom while men inside");

		PeopleQueue.push_back(EGender::Female);

		NoMenInside.wait(lock);

	}

	GenderInside = (GenderInside == EGender::None) ? EGender::Female : GenderInside;

	NumWomen++;
	PrintMessage("Woman walked in");

	PrintWomenInside();
}

void Bathroom::DecrementWomen()
{
	BathroomQueueMutex.lock();
	NumWomen--;

	if (NumWomen == 0)
	{
		GenderInside = EGender::None;
		int counter = 0;
		EGender firstGenderInQueue = GetFirstGenderInQueue();

		while (counter < PeopleQueue.size() && firstGenderInQueue == PeopleQueue[counter])
		{
			counter++;
		}

		for (int i = 0; i < counter; i++)
		{
			if (!PeopleQueue.empty())
			{
				PeopleQueue.pop_front();
			}

			NoWomenInside.notify_one();
		}
	}
	else if (NumWomen < 0)
	{
		PrintError("Less than 0 women");
	}

	PrintMessage("Woman walked out");
	PrintWomenInside();

	BathroomQueueMutex.unlock();
}

Person::~Person()
{
	BathroomPtr = nullptr;
}
