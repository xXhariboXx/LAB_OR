#pragma once

#include <thread>
#include <iostream>
#include <deque>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <windows.h>

namespace
{
	std::mutex BathroomQueueMutex;
}

enum EGender
{
	None,
	Male,
	Female
};

class Bathroom
{

private:

	std::atomic <int> NumWomen = 0;
	std::atomic <int> NumMen = 0;

	EGender GenderInside = EGender::None;
	std::deque<EGender> PeopleQueue;
	int NextGenderCounter = 0;

	std::mutex Mutex;
	std::condition_variable NoWomenInside, NoMenInside;

public:

	Bathroom();
	~Bathroom();

	void IncrementMen();
	void DecrementMen();
	void IncrementWomen();
	void DecrementWomen();

	EGender GetGenderInside() { return GenderInside; }

private:

	EGender GetFirstGenderInQueue();

	void PrintMessage(std::string message)
	{
		std::cout << message.c_str() << std::endl;
	}

	void PrintError(std::string message)
	{
		std::cout << "Error: " << message.c_str() << "!" << std::endl;
	}

	void PrintMenInside()
	{
		std::cout << "Men inside: " << NumMen << std::endl;
	}

	void PrintWomenInside()
	{
		std::cout << "Women inside: " << NumWomen << std::endl;
	}
};

class Person
{

protected:

	EGender Gender;
	Bathroom* BathroomPtr;
	Person* NextPerson;

public:

	Person(EGender gender, Bathroom* bathroomPtr) : Gender(gender), BathroomPtr(bathroomPtr) { NextPerson = nullptr; }
	virtual ~Person();

	virtual void UseBathroom() = 0;

	void SetNextPerson(Person* person) { NextPerson = person; }
};

class Man : public Person
{

public:
	Man(Bathroom* bathroomPtr) : Person(EGender::Male, bathroomPtr) {}
	virtual ~Man() override = default;

	virtual void UseBathroom() override;
};

class Woman : public Person
{

public:
	Woman(Bathroom* bathroomPtr) : Person(EGender::Female, bathroomPtr) {}
	virtual ~Woman() override = default;

	virtual void UseBathroom() override;
};

class BathroomExercise
{

public:

	BathroomExercise();
	~BathroomExercise();
};

