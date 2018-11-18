#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

const int N = 5;

mutex m;
mutex m_scr;

// monitor dla problemu producenta i konsumenta
class Buffer
{
	// rozmiar bufora
	const int size;

	// bufor z danymi
	int* b;

	// pocz¹tek, koniec bufora, liczba elementów w buforze
	int front, end, count;

	mutex m;
	condition_variable not_full, not_empty;

public:
	Buffer(int size) : size(size), front(0), end(0), count(0)
	{
		b = new int[size];
	}

	~Buffer()
	{
		delete[] b;
	}

	// funkcja wywo³ywana przez producenta
	void put(int x)
	{
		unique_lock<mutex> ul(m);

		if (count == size)
			not_full.wait(ul);

		b[end] = x;
		end = (end + 1) % size;
		++count;

		not_empty.notify_one();
	}

	// funkcja wywo³ywana przez konsumenta
	int get()
	{
		unique_lock<mutex> ul(m);
		//utworzenie stra¿nika blokady dla muteksu m i zablokowanie muteksu
		//Dla obiektu klasy unique_lock mo¿liwe jest jawne zarz¹dzanie 
		//powi¹zaniem z muteksem i jego blokowaniem

		if (count == 0)
			not_empty.wait(ul);

		int x = b[front];
		front = (front + 1) % size;
		--count;

		not_full.notify_one();

		return x;
	}
};


// funkcja w¹tku producenta
void f_prod(int id, Buffer& b)
{
	int x;

	srand(time(0) + id);

	do
	{
		x = rand() % 10;

		m_scr.lock();
		cout << "Producer: " << id << ", value: " << x << endl;
		m_scr.unlock();

		b.put(x);
	} while (x);
}

// funkcja w¹tku konsumenta
void f_cons(int id, Buffer& b)
{
	int x;

	do
	{
		x = b.get();

		m_scr.lock();
		cout << "Consumer: " << id << ", value: " << x << endl;
		m_scr.unlock();
	} while (x);
}

//int main()
//{
//	Buffer b(3);
//
//	vector<thread> thr;
//
//	// utworzenie w¹tków konsumenta
//	for (int i = 0; i < N; i++)
//		thr.push_back(thread(f_cons, i, std::ref(b)));
//
//	// utworzenie w¹tków producenta
//	for (int i = 0; i < N; i++)
//		thr.push_back(thread(f_prod, i, std::ref(b)));
//
//	// oczekiwanie na zakoñczenie w¹tków 
//	for (auto& t : thr)
//		t.join();
//
//	return 0;
//}