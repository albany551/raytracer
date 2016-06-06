
#ifndef MATH_RAY_HPP
#define MATH_RAY_HPP

#include <math/vector3.hpp>

namespace math
{
	struct Ray
	{
		Vector3 origin;
		Vector3 direction;

		Ray()
		{
		}

		Ray(const Vector3& a, const Vector3& b) :
            origin(a), 
            direction(Normalize(b-a))
		{
		}
	};
	
} // namespace math

typedef math::Ray ray3f;

#endif
