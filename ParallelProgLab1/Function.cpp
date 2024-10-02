#include <cmath>

class Function
{
public:
	Function();
	~Function();

	double MainFunction(double x)
	{
		return (1 + x) / pow((2 + 3 * x),2);
	}

private:

};

Function::Function()
{
}

Function::~Function()
{
}