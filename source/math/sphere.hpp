
#ifndef MATH_SPHERE_HPP
#define MATH_SPHERE_HPP

#include <math/vector3.hpp>

namespace math
{
	struct Sphere
	{	
		Vector3	origin;
		float	radius;

		Sphere()
		{
		}

		Sphere(const Vector3& c, const float& r)
		: origin(c), radius(r)
		{
		}

		bool Contains(const Vector3& point) const
		{
			return LengthSquared((point-origin)) < (radius*radius);
		}
		
	};

} // namespace math

typedef math::Sphere sphere3f;

#endif
