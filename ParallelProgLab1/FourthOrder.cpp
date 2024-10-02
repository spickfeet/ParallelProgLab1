#include <cmath>
class ThirdOrder
{
public:
    double CalculateIntegral(double step, double lowerLimit, double upperLimit, double* out)
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
                sum += w[i] * (1 + x) / pow((2 + 3 * x), 2);
            }
            result += sum * c0 * step;
            sum = 0;
            if (x + step / 2 > upperLimit)
                break;
        }
        *out = result;
    }
};