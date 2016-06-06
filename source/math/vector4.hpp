
#ifndef MATH_VECTOR4_HPP
#define MATH_VECTOR4_HPP

#include <math/vector3.hpp>

namespace math
{
	struct Vector4
	{
		union {
			struct {
				float x, y, z, w;
			};
			struct {
				float r, g, b, a;
			};
			float v[3];
		};

		Vector4()
		{
		}

		Vector4(const float& vx, const float& vy, const float& vz, const float& vw)
		: x(vx), y(vy), z(vz), w(vw)
		{
		}

		Vector4(const float v[])
		: x(v[0]), y(v[1]), z(v[2]), w(v[3])
		{
		}

		Vector4(const Vector4& v)
		: x(v.x), y(v.y), z(v.z), w(v.w)
		{
		}

		Vector4(const float& v)
		: x(v), y(v), z(v), w(v)
		{
		}

		Vector4 operator + () const
		{
			return *this;
		}

		Vector4 operator - () const
		{
			return Vector4(-x, -y, -z, -w);
		}

		Vector4 operator + (const Vector4& v) const
		{
			return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		Vector3 operator + (const Vector3& v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector4 operator - (const Vector4& v) const
		{
			return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		Vector3 operator - (const Vector3& v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		Vector4 operator * (const Vector4& v) const
		{
			return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
		}

		Vector3 operator * (const Vector3& v) const
		{
			return Vector3(x * v.x, y * v.y, z * v.z);
		}

		Vector4 operator * (const float& s) const
		{
			return Vector4(x * s, y * s, z * s, w * s);
		}

		Vector4 operator / (const float& s) const
		{
			assert( s != 0 );
			float is = 1 / s;
			return Vector4(x * is, y * is, z * is, w * is);
		}

		Vector4& operator += (const Vector4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		Vector4& operator += (const Vector3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector4& operator -= (const Vector4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		Vector4& operator -= (const Vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		Vector4& operator *= (const Vector4& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return *this;
		}

		Vector4& operator *= (const Vector3& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		Vector4& operator *= (const float& s)
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}

		Vector4& operator /= (const float& s)
		{
			assert( s != 0 );
			float is = 1 / s;
			x *= is;
			y *= is;
			z *= is;
			w *= is;
			return *this;
		}

		Vector4& operator = (const Vector4& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
			return *this;
		}

		Vector4& operator = (const float& v)
		{
			x = v;
			y = v;
			z = v;
			w = v;
			return *this;
		}

		inline float &operator[](int i) { return ((float*)&x)[i]; }
		inline const float operator[](int i) const { return ((float*)&x)[i]; }

		void Normalize()
		{
			float s = x*x + y*y + z*z + w*w;
			if ( s != 0 )
			{
				s = 1 / Sqrt(s);
				x *= s;
				y *= s;
				z *= s;
				w *= s;
			}
		}
	};

	inline Vector3 operator * (const Vector3& a, const Vector4& b)
	{
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	inline Vector4 operator * (const float& s, const Vector4& v)
	{
		return Vector4(s * v.x, s * v.y, s * v.z, s * v.w);
	}

	inline float LengthSquared(const Vector4& v)
	{
		return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;
	}

	inline float Length(const Vector4& v)
	{
		return static_cast<float>(Sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w));
	}

	inline float DotProduct(const Vector4& a, const Vector4& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	inline Vector4 Normalize(const Vector4& v)
	{
		float s = v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;
		if ( s == 0 )
			return v;
		
		s = 1 / static_cast<float>(Sqrt(s));
		return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
	}


} // namespace math

typedef math::Vector4 vector4f;

#endif
