#include "Function.cpp"
class FourthOrder
{
public:
	FourthOrder(Function func) 
	{
		_func = func;
	}
	~FourthOrder() 
	{
        _func.~Function();
	}
    double CalculateIntegral(double step, double lowerLimit, double upperLimit)
    {
        double x = lowerLimit;
        double c0 = 3.0 / 8.0;
        double w[4] = { 1, 3, 3, 1 };
        double sum = 0;
        double result = 0;
        while (true)
        {
            x -= step;
            for (int i = 0; i < 4; i++)
            {
                x += step;
                sum += w[i] * _func.MainFunction(x);
            }
            result += sum * c0 * step;
            sum = 0;
            if (x + step / 2 > upperLimit)
                break;
        }
        return result;
    }

private:
	Function _func;
};