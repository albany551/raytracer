
#ifndef MATH_INTERSECT_HPP
#define MATH_INTERSECT_HPP

#include <math/ray.hpp>
#include <math/sphere.hpp>
#include <math/box.hpp>

namespace math
{
    bool IntersectRaySphere(const Ray& ray, const sphere3f& sphere, float& enter, Vector3& normal);

	// =====================================
	// Based on article by Tomas Möller
	// Fast, Minimum Storage Ray-Triangle Intersection
	// =====================================
	bool IntersectRayTriangle(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& enter, float& u, float& v);

	// =====================================
	// Kay and Kayjia "slab" method
	// =====================================
	bool IntersectRayBox(const Ray& ray, const Box& box, float& enter, float& leave);



	inline bool IntersectRaySphere(const Ray& ray, const sphere3f& sphere, float& enter)
	{
		Vector3 normal;
		return IntersectRaySphere(ray, sphere, enter, normal);
	}

	inline bool IntersectRayTriangle(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& enter)
	{
		float b0, b1;
		return IntersectRayTriangle(ray, v0, v1, v2, enter, b0, b1);
	}

	inline bool IntersectRayBox(const Ray& ray, const Box& box)
	{
		float enter, leave;
		return IntersectRayBox(ray, box, enter, leave);
	}




} // namespace math

#endif
