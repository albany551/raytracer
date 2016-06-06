
#include <math/common.hpp>

#include <cmath>
#include <limits>

namespace math
{
	const float INFINITY = std::numeric_limits<float>::infinity();
	const float FLOATMAX  = std::numeric_limits<float>::max();
	const float FLOATMIN = std::numeric_limits<float>::min();
	const float EPSILON = std::numeric_limits<float>::epsilon();

	int Abs(int a)
	{
		return ::abs(a);
	}

	float Abs(float a)
	{
		return ::fabsf(a);
	}

	float Sqrt(float a)
	{
		return sqrtf(a);
	}

	float Sin(float a)
	{
		return ::sinf(a);
	}

	float Cos(float a)
	{
		return ::cosf(a);
	}

	float Tan(float a)
	{
		return ::tanf(a);
	}

	float ASin(float a)
	{
		return ::asinf(a);
	}

	float ACos(float a)
	{
		return ::acosf(a);
	}

	float ATan(float a)
	{
		return ::atanf(a);
	}

	float ATan(float a, float b)
	{
		return ::atan2f(a, b);
	}

	float Pow(float a, int b)
	{
		return ::pow(a, b);
	}

	float Pow(float a, float b)
	{
		return ::pow(a, b);
	}

}


