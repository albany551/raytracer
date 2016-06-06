
#ifndef RAYTRACE_HITINFO_HPP
#define RAYTRACE_HITINFO_HPP

#include <math/vector2.hpp>
#include <math/vector3.hpp>
#include <math/matrix44.hpp>
#include <math/ray.hpp>
#include <scene/visual.hpp>

namespace raytrace
{
	using scene::Visual;

	struct HitInfo
	{
		HitInfo() :
			success(false),
			inside(false),
			visual(0)
		{
			distance = math::INFINITY;
		}

		vector3f	point;
		vector3f	normal;
		vector2f	texcoord;
		float		distance;
		Visual*		visual;
		bool		success;
		bool		inside;

	};

	bool FindCloserIntersection(HitInfo& result, const ray3f& ray, const matrix44f& worldtm, Visual* visual);

}


#endif
