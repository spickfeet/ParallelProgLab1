#include "pthread/pthread.h"
#include <string>
#include <iostream>
#include <ctime>
#include <windows.h>
#include <queue>
#include <fstream>
#include <mutex>
#pragma comment(lib,"pthread/pthreadVCE2.lib")

using namespace std;

// ЛАБ 1

//typedef struct IntegralParams
//{
//    double step;
//    double lowerLimit;
//    double upperLimit;
//    double out;
//};
//
//void* CalculateIntegral(void* args)
//{
//    IntegralParams *params = (IntegralParams*)args;
//    double x = params->lowerLimit;
//    double c0 = 3.0 / 8.0;
//    double w[4] = { 1, 3, 3, 1 };
//    double sum = 0;
//    double result = 0;
//    
//    while (true)
//    {
//        x -= params->step;
//        for (int i = 0; i < 4; i++)
//        {
//            x += params->step;
//            sum += w[i] * (1 + x) / (2 + 3 * x);
//        }
//        result += sum * c0 * params->step;
//        sum = 0;
//        if (x + params->step / 2 > params->upperLimit)
//            break;
//    }
//    params->out = result;
//    return NULL;
//}
//
//void* qwerty(void* out)
//{
//	double* res = (double*)out;
//	*res = 5;
//	return NULL;
//}
//
//int main() 
//{
//    double res = 0;
//
//    #ifdef _WIN32
//        SetConsoleCP(CP_UTF8);
//        SetConsoleOutputCP(CP_UTF8);
//    #endif
//    
//    int countThreads = 5;
//    double step;
//    double lowerLimit;
//    double upperLimit;
//
//    cout << "Введите через пробел: минимальный и максимальный предел интегрирования, количество потоков (от 1 до 8) и величину шага" << '\n';
//    cin >> lowerLimit >> upperLimit >> countThreads >> step;
//
//    clock_t start_time = clock();
//    
//    IntegralParams* params = new IntegralParams[countThreads];
//
//    pthread_t* threads = new pthread_t[countThreads];
//    
//
//    double length = (upperLimit - lowerLimit) / countThreads;
//
//    for (int i = 0; i < countThreads; i++)
//    {
//        params[i].step = step;
//        params[i].lowerLimit = lowerLimit + length * i;
//        params[i].upperLimit = params[i].lowerLimit + length;
//
//        pthread_create(&threads[i], NULL, CalculateIntegral, &params[i]);
//    }
//
//    for (int i = 0; i < countThreads; i++)
//    {
//        pthread_join(threads[i], NULL);
//    }
//
//    for (int i = 0; i < countThreads; i++)
//    {
//        res += params[i].out;
//    }
//
//	cout << res << endl;
//    cout << "Затраченное время: " << static_cast<float>(clock() - start_time)/CLOCKS_PER_SEC << " секунды" << endl;
//    
//	return 0;
//}

// ЛАБ 2

template<typename T>
class Queue
{
public:

	void push(T arg)
	{
		mutex.lock();

		queue.push(arg);

		mutex.unlock();
	}

	T pop()
	{
		mutex.lock();

		T res = queue.front();
		queue.pop();

		mutex.unlock();
		return res;
	}

	bool empty() 
	{

		bool res = queue.empty();

		return res;
	}

	size_t size() 
	{
		mutex.lock();

		size_t res = queue.size();

		mutex.unlock();
		return res;
	}

private:
	std::queue<T> queue;
	mutex mutex;

};

class File {
public:
	int countThreadStopRead = 0;
	File(string readName, string writeName)
	{
		readStream.open(readName, ios_base::in);
		writeStream.open(writeName, ios_base::out);
	}
	int readNumber()
	{
		readMutex.lock();
		int number;
		readStream >> number;
		if (readStream.fail()) {
			readMutex.unlock();
			return -1;
		}
		readMutex.unlock();
		return number;
	}

	void writeResult(string result)
	{
		writeMutex.lock();
		writeStream << result << endl;
		writeMutex.unlock();
	}
	~File()
	{
		readStream.close();
		writeStream.close();
	}

private:
	std::mutex readMutex;
	std::mutex writeMutex;
	std::fstream readStream;
	std::fstream writeStream;
};

typedef struct Params
{
	File* fileWorker;
	Queue<string>* queue;
};

bool isPrime(int n)
{
	if (n > 1)
	{
		for (int i = 2; i < n; i++)
			if (n % i == 0)
				return false;
		return true;
	}
	else
		return false;
}


void* readAndCheck(void* args)
{
	Params* params = (Params*)args;
	while (true)
	{

		int num = params->fileWorker->readNumber();
		if (num != -1)
		{
			params->queue->push(to_string(num) + (isPrime(num) == true ? " Да" : " Нет"));
			
		}
		else 
		{
			params->fileWorker->countThreadStopRead += 1;
			return NULL;
		}
	}
}

void* write(void* args)
{
	Params* params = (Params*)args;
	while (params->fileWorker->countThreadStopRead != 2 or params->queue->empty() == false)
	{		

		if (params->queue->empty()) {
			continue;
		}
		string value = params->queue->pop();
		params->fileWorker->writeResult(value);		
	}
	return NULL;	


}


int main()
{
	File* fileWorker = new File("read.txt", "result.txt");
	Queue<string>* queue1 = new Queue<string>();
	Queue<string>* queue2 = new Queue<string>();

	int b = 3;

	Params* params = new Params[2];

	params[0].fileWorker = fileWorker;
	params[0].queue = queue1;

	params[1].fileWorker = fileWorker;
	params[1].queue = queue2;

	pthread_t* threads = new pthread_t[4];

	pthread_create(&threads[0], NULL, readAndCheck, &params[0]);
	pthread_create(&threads[1], NULL, write, &params[0]);

	pthread_create(&threads[2], NULL, readAndCheck, &params[1]);
	pthread_create(&threads[3], NULL, write, &params[1]);
	
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	pthread_join(threads[3], NULL);
}
