#include "pthread/pthread.h"
#include <iostream>
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

    const int countP = 5;
    double step = 0.01;
    double lowerLimit = 0;
    double upperLimit = 10;

    IntegralParams params[countP];

    pthread_t pthread[countP];
    

    int length = (upperLimit - lowerLimit) / countP;

    for (int i = 0; i < countP; i++)
    {
        params[i].step = step;
        params[i].lowerLimit = lowerLimit + length * i;

        params[i].upperLimit = params[i].lowerLimit + length;
        //CalculateIntegral(&params[i]);
        pthread_create(&pthread[i], NULL, CalculateIntegral, &params[i]);
    }

    for (int i = 0; i < countP; i++)
    {
        pthread_join(pthread[i], NULL);
    }

    for (int i = 0; i < countP; i++)
    {
        res += params[i].out;
    }

	cout << res << endl;
	return 0;
}
