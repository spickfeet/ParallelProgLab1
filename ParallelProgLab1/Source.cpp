#include "pthread/pthread.h"
#include <iostream>
#include <ctime>
#include <windows.h>
#pragma comment(lib,"pthread/pthreadVCE2.lib")
using namespace std;

typedef struct IntegralParams
{
    double step;
    double lowerLimit;
    double upperLimit;
    double out;
};

void* CalculateIntegral(void* args)
{
    IntegralParams *params = (IntegralParams*)args;
    double x = params->lowerLimit;
    double c0 = 3.0 / 8.0;
    double w[4] = { 1, 3, 3, 1 };
    double sum = 0;
    double result = 0;
    
    while (true)
    {
        x -= params->step;
        for (int i = 0; i < 4; i++)
        {
            x += params->step;
            sum += w[i] * (1 + x) / (2 + 3 * x);
        }
        result += sum * c0 * params->step;
        sum = 0;
        if (x + params->step / 2 > params->upperLimit)
            break;
    }
    params->out = result;
    return NULL;
}

void* qwerty(void* out)
{
	double* res = (double*)out;
	*res = 5;
	return NULL;
}

int main() 
{
    double res = 0;

    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif
    
    int countThreads = 5;
    double step;
    double lowerLimit;
    double upperLimit;

    cout << "Введите через пробел: минимальный и максимальный предел интегрирования, количество потоков (от 1 до 8) и величину шага" << '\n';
    cin >> lowerLimit >> upperLimit >> countThreads >> step;

    clock_t start_time = clock();
    
    IntegralParams* params = new IntegralParams[countThreads];

    pthread_t* threads = new pthread_t[countThreads];
    

    double length = (upperLimit - lowerLimit) / countThreads;

    for (int i = 0; i < countThreads; i++)
    {
        params[i].step = step;
        params[i].lowerLimit = lowerLimit + length * i;
        params[i].upperLimit = params[i].lowerLimit + length;

        pthread_create(&threads[i], NULL, CalculateIntegral, &params[i]);
    }

    for (int i = 0; i < countThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < countThreads; i++)
    {
        res += params[i].out;
    }

	cout << res << endl;
    cout << "Затраченное время: " << static_cast<float>(clock() - start_time)/CLOCKS_PER_SEC << " секунды" << endl;
    
	return 0;
}
