
#ifndef MATH_VECTOR3_HPP
#define MATH_VECTOR3_HPP

#include <math/common.hpp>

namespace math
{
	struct Vector3
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};
			struct
			{
				float r;
				float g;
				float b;
			};
			float v[3];
		};

		Vector3()
		{
		}

		Vector3(const float& vx, const float& vy, const float& vz)
		: x(vx), y(vy), z(vz)
		{
		}

		Vector3(const float v[])
		: x(v[0]), y(v[1]), z(v[2])
		{
		}

		Vector3(const Vector3& v)
		: x(v.x), y(v.y), z(v.z)
		{
		}

		Vector3(const float& v)
		: x(v),y(v),z(v)
		{
		}

		Vector3 operator + () const
		{
			return *this;
		}

		Vector3 operator - () const
		{
			return Vector3(-x, -y, -z);
		}

		Vector3 operator + (const Vector3& v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector3 operator - (const Vector3& v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		Vector3 operator * (const Vector3& v) const
		{
			return Vector3(x * v.x, y * v.y, z * v.z);
		}

		Vector3 operator * (const float& s) const
		{
			return Vector3(x * s, y * s, z * s);
		}

		Vector3 operator / (const float& s) const
		{
			assert( s != 0 );
			float is = 1 / s;
			return Vector3(x * is, y * is, z * is);
		}

		Vector3& operator += (const Vector3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector3& operator -= (const Vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		Vector3& operator *= (const Vector3& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		Vector3& operator *= (const float& s)
		{
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}

		Vector3& operator /= (const float& s)
		{
			assert( s != 0 );
			float is = 1 / s;
			x *= is;
			y *= is;
			z *= is;
			return *this;
		}

		Vector3& operator = (const Vector3& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		Vector3& operator = (const float& v)
		{
			x = v;
			y = v;
			z = v;
			return *this;
		}

		inline float &operator[](int i) { return ((float*)&x)[i]; }
		inline const float operator[](int i) const { return ((float*)&x)[i]; }

		void Normalize()
		{
			float s = x*x + y*y + z*z;
			if ( s != 0 )
			{
				s = 1 / Sqrt(s);
				x *= s;
				y *= s;
				z *= s;
			}
		}
	};

	inline Vector3 operator * (const float& s, const Vector3& v)
	{
		return Vector3(s * v.x, s * v.y, s * v.z);
	}

	inline Vector3 operator * (const int& s, const Vector3& v)
	{
		return Vector3(s * v.x, s * v.y, s * v.z);
	}

	inline float LengthSquared(const Vector3& v)
	{
		return v.x*v.x + v.y*v.y + v.z*v.z;
	}

	inline float Length(const Vector3& v)
	{
		return static_cast<float>(Sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
	}

	inline float DotProduct(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline Vector3 CrossProduct(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			(a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x));
	}

	inline Vector3 Normalize(const Vector3& v)
	{
		float s = v.x*v.x + v.y*v.y + v.z*v.z;
		if ( s == 0 )
			return v;
		
		s = 1 / static_cast<float>(Sqrt(s));
		return Vector3(v.x * s, v.y * s, v.z * s);
	}

	inline Vector3 TriangleNormal(const Vector3& v0, const Vector3& v1, const Vector3& v2)
	{
		return Normalize(CrossProduct(v1-v0,v2-v0));
	}

	inline bool WithinTolerance(const Vector3& a, const Vector3& b, float tolerance)
	{
		if( Abs(a.x-b.x) > tolerance) return false;
		if( Abs(a.y-b.y) > tolerance) return false;
		if( Abs(a.z-b.z) > tolerance) return false;
		return true;
	}

} // namespace math

typedef math::Vector3 vector3f;
typedef math::Vector3 color3f;

#endif
