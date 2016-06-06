
#ifndef MATH_BOX_HPP
#define MATH_BOX_HPP


#include <math/common.hpp>
#include <math/vector3.hpp>
#include <math/matrix44.hpp>

namespace math
{
	struct Box
	{

		Vector3 vmin;
		Vector3 vmax;


		Box() :
			vmin(FLOATMAX, FLOATMAX, FLOATMAX),
			vmax(-FLOATMAX, -FLOATMAX, -FLOATMAX)
		{
		}

		Box(const Vector3& umin, const Vector3& umax) : 
			vmin(umin), vmax(umax)
		{
		}

		Box(const Box& a, const Box& b)
		{
			vmin.x = a.vmin.x < b.vmin.x ? a.vmin.x : b.vmin.x;
			vmin.y = a.vmin.y < b.vmin.y ? a.vmin.y : b.vmin.y;
			vmin.z = a.vmin.z < b.vmin.z ? a.vmin.z : b.vmin.z;
			vmax.x = a.vmax.x > b.vmax.x ? a.vmax.x : b.vmax.x;
			vmax.y = a.vmax.y > b.vmax.y ? a.vmax.y : b.vmax.y;
			vmax.z = a.vmax.z > b.vmax.z ? a.vmax.z : b.vmax.z;
		}

		void FitBox(const Box& a)
		{
			FitBox(a.vmin, a.vmax);
		}

		void FitBox(const Vector3& umin, const Vector3& umax)
		{
			if ( umin.x < vmin.x) vmin.x = umin.x;
			if ( umin.y < vmin.y) vmin.y = umin.y;
			if ( umin.z < vmin.z) vmin.z = umin.z;
			if ( umax.x > vmax.x) vmax.x = umax.x;
			if ( umax.y > vmax.y) vmax.y = umax.y;
			if ( umax.z > vmax.z) vmax.z = umax.z;
		}

		void FitPoint(const Vector3& v)
		{
			if ( v.x < vmin.x ) vmin.x = v.x; if ( v.x > vmax.x ) vmax.x = v.x;
			if ( v.y < vmin.y ) vmin.y = v.y; if ( v.y > vmax.y ) vmax.y = v.y;
			if ( v.z < vmin.z ) vmin.z = v.z; if ( v.z > vmax.z ) vmax.z = v.z;
		}

		Vector3 GetCenter() const
		{
			return (vmin + vmax) * 0.5f;
		}

		Vector3 GetSize() const
		{
			return vmax - vmin;
		}

		bool Contains(const Vector3& point) const
		{
			if ( point.x < vmin.x || point.y < vmin.y || point.z < vmin.z ) return false;
			if ( point.x > vmax.x || point.y > vmax.y || point.z > vmax.z ) return false;
			return true;
		}

		void GetPoints(Vector3 vertex[]) const
		{
			vertex[0] = Vector3(vmin.x,vmin.y,vmin.z);
			vertex[1] = Vector3(vmax.x,vmin.y,vmin.z);
			vertex[2] = Vector3(vmin.x,vmax.y,vmin.z);
			vertex[3] = Vector3(vmax.x,vmax.y,vmin.z);
			vertex[4] = Vector3(vmin.x,vmin.y,vmax.z);
			vertex[5] = Vector3(vmax.x,vmin.y,vmax.z);
			vertex[6] = Vector3(vmin.x,vmax.y,vmax.z);
			vertex[7] = Vector3(vmax.x,vmax.y,vmax.z);
		}

		void GetPoints(Vector3 vertex[], const Matrix44& transform) const
		{
			vertex[0] = transform * Vector3(vmin.x,vmin.y,vmin.z);
			vertex[1] = transform * Vector3(vmax.x,vmin.y,vmin.z);
			vertex[2] = transform * Vector3(vmin.x,vmax.y,vmin.z);
			vertex[3] = transform * Vector3(vmax.x,vmax.y,vmin.z);
			vertex[4] = transform * Vector3(vmin.x,vmin.y,vmax.z);
			vertex[5] = transform * Vector3(vmax.x,vmin.y,vmax.z);
			vertex[6] = transform * Vector3(vmin.x,vmax.y,vmax.z);
			vertex[7] = transform * Vector3(vmax.x,vmax.y,vmax.z);
		}
	};

} // namespace math

typedef math::Box box3f;

#endif