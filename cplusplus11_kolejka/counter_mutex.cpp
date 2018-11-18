#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

struct Counter
{
	int value;
	std::mutex my_mutex;
	Counter() :value(0) {}

	void increment()
	{
		my_mutex.lock();
		++value;
		my_mutex.unlock();
	}
};

//int main()
//{
//	Counter counter;
//	std::vector<std::thread> my_threads;
//
//	for (int i = 0; i < 4; ++i)
//	{
//		my_threads.push_back(std::thread([&counter]() {
//			for (int i = 0; i < 5000; ++i) {
//				counter.increment();
//			}
//		}));
//	}
//	for (auto & thread : my_threads) {
//		thread.join();
//	}
//	std::cout << counter.value << std::endl;
//
//	return 0;
//
//}