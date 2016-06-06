
#include <math/intersect.hpp>

using namespace math;

namespace math
{
    bool IntersectRaySphere(const Ray& ray, const Sphere& sphere, float& enter, Vector3& normal)
    { 
    	Vector3 dst = ray.origin - sphere.origin;
    	float B = 2.0f * DotProduct(dst,ray.direction);
    	float C = LengthSquared(dst) - (sphere.radius*sphere.radius);
    	float discr = B*B - 4.0f * C;
    	if(discr >= 0)
    	{
        	float t0 = (-B - Sqrt(discr)) * 0.5f;
        	if(t0 >= 0)
			{
				enter = t0;
				normal = Normalize((ray.origin + t0 * ray.direction) - sphere.origin);
				return true;
			}
            //float t1 = (-B + sqrt(discr)) * 0.5f;
        	//if(t1 >= 0) return t1;
        }
    	return false;
    }

	bool IntersectRayTriangle(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& enter, float& u, float& v)
	{
		const float epsilon = static_cast<float>(0.000001);

		Vector3 edge1 = v1 - v0;
		Vector3 edge2 = v2 - v0;

		Vector3 pvec = CrossProduct( ray.direction,edge2 );
		float det = DotProduct(edge1, pvec );

		if( det > -epsilon && det < epsilon ) return false;

		det = 1.0f / det;

		Vector3 tvec = ray.origin - v0;
		u = DotProduct( tvec,pvec ) * det;
		if( u < 0.0 || u > 1.0 )
			return false;

		Vector3 qvec = CrossProduct(tvec, edge1 );
		v = DotProduct(ray.direction, qvec ) * det;
		if( v < 0.0f || u + v > 1.0f )
			return 0;

		enter = DotProduct( edge2,qvec ) * det;

		return true;
	}


	bool IntersectRayBox(const Ray& ray, const Box& box, float& enter, float& leave)
	{
		float tnear = -FLOATMAX;
		float tfar = FLOATMAX;
	
		for ( int i=0; i<3; ++i )
		{
			float origin = ray.origin[i];
			float direction = ray.direction[i];
			float vmin = box.vmin[i];
			float vmax = box.vmax[i];
		
			if ( direction )
			{
				// calculate intersections to the slab
				direction = 1 / direction;
				float t1 = (vmin - origin) * direction;
				float t2 = (vmax - origin) * direction;
			
				if ( t1 > t2 )
				{
					if ( t2 > tnear ) tnear = t2;
					if ( t1 < tfar ) tfar = t1;
				}
				else
				{
					if ( t1 > tnear ) tnear = t1;
					if ( t2 < tfar ) tfar = t2;
				}
		
				// failed intersecting this axis or intersecting behind the origin
				if ( tnear > tfar || tfar < 0 )
					return false;
			}
			else
			{
				// ray parallel to axis/slab being tested against
				if ( origin < vmin || origin > vmax )
					return false;
			}
		}

		// intersection result
		enter = tnear;
		leave = tfar;

		// ray hits box
		return true;
	}
}