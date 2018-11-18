// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <algorithm>	  // std::max
#include <vector>		  // std::vector


void hobbit()
{
	std::cout << "Podrowienia od Hobbita\n";
}

void elf(int x)
{
	for (int i = 0; i < x; i++)
	 std::cout << "Podrowienia od Elfa\n";
}

class Tajna_misja
{
public:
	void operator()() const
	{
		std::cout << "Nikt o tym nie wie..." << std::endl;
	}
};

class Druga_tajna_misja
{
	int x_;
	double y_;

public:
	Druga_tajna_misja(int x, double y) : x_(x), y_(y) {}

	void operator()()
	{
		std::cout << "Nikt nie wie o " << x_ << " ani o " << y_ << std::endl;
	}
};

void parametr_przez_wartosc(int n)
{
	for (int i = 0; i < 5; ++i)
	{
		std::cout << "inkrementuje pierwszy watek...\n";
		++n; // increment of local copy
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void parametr_przez_referencje(int& n)
{
	for (int i = 0; i < 5; ++i)
	{
		std::cout << "inkrementuje drugi watek\n";
		++n;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

//int main()
//{
//	//Tworzenie w�tk�w przez przekazanie wska�nika do funkcji
//	std::thread t1(hobbit);     // powo�uje nowy w�tek, kt�ry wywo�uje hobbit()
//	std::thread t2(elf, 2);  // powo�uje nowy w�tek, kt�ry wywo�uje elf()
//
//	std::cout << "main, hobbit i elf dzia�aj� wspo�bie�nie...\n";
//
//	// synchronizacja w�tk�w:
//	t1.join();                // wstrzymanie wykonania funkcji main
//	t2.join();               // a� do czasu zako�czenia w�tk�w t1 i t2
//              
//							 //The join function here is to force the current thread 
//							 //to wait for the thread to th finish.
//							 //Otherwise the main function may exit without the thread th finished
//
//	std::cout << "hobbit i elf zako�czyli prac�.\n";
//
//	Tajna_misja t_m;
//	std::thread t3(t_m);
//	std::thread t4((Tajna_misja()));
//	t3.join();
//	t4.join();
//
//	std::thread t5([] { std::cout << "Tak te� mo�na..." << std::endl; });
//	t5.join();
//
//	std::cout << "hardware_concurrency() = "
//		<< std::thread::hardware_concurrency() << std::endl;
//
//	//******************************************************
//	//kontenery w�tk�w
//	//******************************************************
//	auto ile_fabryka_da�a = std::max(1u, std::thread::hardware_concurrency());
//	
//	std::vector<std::thread> w�tki_w_kontenerze;
//	for (int i = 0; i < ile_fabryka_da�a; ++i) 
//	{
//		w�tki_w_kontenerze.push_back(std::thread(hobbit));
//	}
//
//	for (auto &t : w�tki_w_kontenerze) 
//	{
//		t.join();
//	};
//
//	//******************************************************
//	//Przekazywanie parametr�w do w�tk�w
//	//******************************************************
//
//	Druga_tajna_misja dtm(1500, 900);
//	std::thread t6(dtm);
//	t6.join();
//
//	int n = 0;
//	std::thread t7(parametr_przez_wartosc, n + 1); // pass by value
//	std::thread t8(parametr_przez_referencje, std::ref(n)); // pass by reference
//	t7.join();
//	t8.join();
//	std::cout << "wynikowa wartosc n to: " << n << '\n';
//	return 0;
//}