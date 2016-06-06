
#ifndef MATH_COMMON_HPP
#define MATH_COMMON_HPP

#include <cassert>

namespace math
{
    const float PI = 3.14159265358979323846264338327950288419716939937510f;

	extern const float INFINITY;
	extern const float FLOATMAX;
	extern const float FLOATMIN;
	extern const float EPSILON;

	int Abs(int a);
	float Abs(float a);
	float Sqrt(float a);

	float Sin(float a);
	float Cos(float a);
	float Tan(float a);

	float ASin(float a);
	float ACos(float a);
	float ATan(float a);
	float ATan(float a, float b);

	float Pow(float a, int b);
	float Pow(float a, float b);

	template <typename T>
	inline T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	inline T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template <typename T>
	inline void Swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}
	
	inline float ToRadians(float angle)
	{
        return static_cast<float>(angle * (PI / 180.0));
    }
    
    inline float ToDegrees(float angle)
    {
        return static_cast<float>(angle * (180.0 / PI));
    }

}

#endif
