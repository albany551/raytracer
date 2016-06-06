
#ifndef MATH_VECTOR2_HPP
#define MATH_VECTOR2_HPP

#include <math/common.hpp>

namespace math
{
	struct Vector2
	{
		union
		{
			struct
			{
				float x;
				float y;
			};
			struct
			{
				float u;
				float v;
			};
			float a[2];
		};

		Vector2()
		{
		}

		Vector2(const float& vx, const float& vy)
		: x(vx), y(vy)
		{
		}

		Vector2(const float v[])
		: x(v[0]), y(v[1])
		{
		}

		Vector2(const Vector2& v)
		: x(v.x), y(v.y)
		{
		}

		Vector2(const float& v)
		: x(v),y(v)
		{
		}

		Vector2 operator + () const
		{
			return *this;
		}

		Vector2 operator - () const
		{
			return Vector2(-x, -y);
		}

		Vector2 operator + (const Vector2& v) const
		{
			return Vector2(x + v.x, y + v.y);
		}

		Vector2 operator - (const Vector2& v) const
		{
			return Vector2(x - v.x, y - v.y);
		}

		Vector2 operator * (const Vector2& v) const
		{
			return Vector2(x * v.x, y * v.y);
		}

		Vector2 operator * (const float& s) const
		{
			return Vector2(x * s, y * s);
		}

		Vector2 operator / (const float& s) const
		{
			assert( s != 0 );
			float is = 1 / s;
			return Vector2(x * is, y * is);
		}

		Vector2& operator += (const Vector2& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		Vector2& operator -= (const Vector2& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		Vector2& operator *= (const Vector2& v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}

		Vector2& operator *= (const float& s)
		{
			x *= s;
			y *= s;
			return *this;
		}

		Vector2& operator /= (const float& s)
		{
			assert( s != 0 );
			float is = 1 / s;
			x *= is;
			y *= is;
			return *this;
		}

		Vector2& operator = (const Vector2& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		Vector2& operator = (const float& v)
		{
			x = v;
			y = v;
			return *this;
		}

		inline float &operator[](int i) { return ((float*)&a)[i]; }
		inline const float operator[](int i) const { return ((float*)&a)[i]; }

		void Normalize()
		{
			float s = x*x + y*y;
			if ( s != 0 )
			{
				s = 1 / Sqrt(s);
				x *= s;
				y *= s;
			}
		}
	};

	inline Vector2 operator * (const float& s, const Vector2& v)
	{
		return Vector2(s * v.x, s * v.y);
	}

	inline Vector2 operator * (const int& s, const Vector2& v)
	{
		return Vector2(s * v.x, s * v.y);
	}

	inline float LengthSquared(const Vector2& v)
	{
		return v.x*v.x + v.y*v.y;
	}

	inline float Length(const Vector2& v)
	{
		return static_cast<float>(Sqrt(v.x*v.x + v.y*v.y));
	}

	inline float DotProduct(const Vector2& a, const Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline Vector2 Normalize(const Vector2& v)
	{
		float s = v.x*v.x + v.y*v.y;
		if ( s == 0 )
			return v;
		
		s = 1 / static_cast<float>(Sqrt(s));
		return Vector2(v.x * s, v.y * s);
	}

} // namespace math

typedef math::Vector2 vector2f;

#endif
