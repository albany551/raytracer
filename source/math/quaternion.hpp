
#ifndef MATH_QUATERNION_HPP
#define MATH_QUATERNION_HPP

#include <math/matrix44.hpp>

namespace math
{

	struct Quaternion
	{
		// members

		float x;
		float y;
		float z;
		float w;

		// constructors

		Quaternion()
		{
		}

		Quaternion(const float& qx, const float& qy, const float& qz, const float& qw)
		: x(qx), y(qy), z(qz), w(qw)
		{
		}

		Quaternion(const Quaternion& q)
		: x(q.x), y(q.y), z(q.z), w(q.w)
		{
		}

		Quaternion(Vector3 v)
		{
			v.Normalize();

			// create axis vector
			Vector3	Av(v.y, -v.x, 0.0f);
			Av.Normalize();

			// calculate angle between target and look vectors
			float angle = ACos(v.z) / 2;

			// build target quaternion
			x = Sin(angle) * Av.x;
			y = Sin(angle) * Av.y;
			z = 0.0f;
			w = Cos(angle);

			Normalize();
		}

		Quaternion(const float& angle, const Vector3& axis)
		{
			SetAngleAxis(angle,axis);
		}

		Quaternion(const Matrix44& m)
		{
			float trace = m.m44[0][0] + m.m44[1][1] + m.m44[2][2];
			float	s;

			if (trace > 0.0f)
			{
				s = (float)Sqrt(trace + 1.0F);
				w = s * 0.5f;
				s = 0.5f / s;
			    
				x = (m.m44[1][2] - m.m44[2][1]) * s;
				y = (m.m44[2][0] - m.m44[0][2]) * s;
				z = (m.m44[0][1] - m.m44[1][0]) * s;
			} 
			else 
			{
				int     i, j, k;
				int 	next[3] = {1, 2, 0};
			    
				i = 0;
				if (m.m44[1][1] > m.m44[0][0]) i = 1;
				if (m.m44[2][2] > m.m44[i][i]) i = 2;
				j = next[i];  
				k = next[j];
			    
				s = (float)Sqrt((m.m44[i][i] - (m.m44[j][j]+m.m44[k][k])) + 1.0f);
				if( i == 0) x = s*0.5f;
				if( i == 1) y = s*0.5f;
				if( i == 2) z = s*0.5f;
			    
				s = 0.5f / s;
			    
				w = (m.m44[j][k] - m.m44[k][j]) * s;
				if(j == 0) x = (m.m44[i][j] + m.m44[j][i]) * s;
				if(j == 1) y = (m.m44[i][j] + m.m44[j][i]) * s;
				if(j == 2) z = (m.m44[i][j] + m.m44[j][i]) * s;

				if(k == 0) x = (m.m44[i][k] + m.m44[k][i]) * s;
				if(k == 1) y = (m.m44[i][k] + m.m44[k][i]) * s;
				if(k == 2) z = (m.m44[i][k] + m.m44[k][i]) * s;
			}
		}

		// operators

		Quaternion operator + () const
		{
			return *this;
		}

		Quaternion operator - () const
		{
			return Quaternion(-x, -y, -z, -w);
		}

		Quaternion operator + (const Quaternion& q) const
		{
			return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
		}

		Quaternion operator - (const Quaternion& q) const
		{
			return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
		}

		Quaternion operator * (const Quaternion& q) const
		{
			return Quaternion(
				w*q.x + x*q.w + y*q.z - z*q.y,
				w*q.y + y*q.w + z*q.x - x*q.z,
				w*q.z + z*q.w + x*q.y - y*q.x,
				w*q.w - x*q.x - y*q.y - z*q.z);
		}

		Quaternion operator * (const float& s) const
		{
			return Quaternion(x * s, y * s, z * s, w * s);
		}

		Quaternion& operator += (const Quaternion& q)
		{
			x += q.x;
			y += q.y; 
			z += q.z;
			w += q.w;
			return *this;
		}

		Quaternion& operator -= (const Quaternion& q)
		{
			x -= q.x;
			y -= q.y; 
			z -= q.z;
			w -= q.w;
			return *this;
		}

		Quaternion& operator *= (const Quaternion& q)
		{
			float sx = w*q.x + x*q.w + y*q.z - z*q.y;
			float sy = w*q.y + y*q.w + z*q.x - x*q.z;
			float sz = w*q.z + z*q.w + x*q.y - y*q.x;
			float sw = w*q.w - x*q.x - y*q.y - z*q.z;
			x = sx;
			y = sy;
			z = sz;
			w = sw;
			return *this;
		}

		Quaternion& operator *= (const float& s)
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}

		Quaternion& operator = (const Quaternion& q)
		{
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
			return *this;
		}

		bool operator == (const Quaternion& q) const
		{
			return x == q.x && y == q.y && z == q.z && w == q.w;
		}

		bool operator != (const Quaternion& q) const
		{
			return x != q.x || y != q.y || z != q.z || w != q.w;
		}

		// methods

		void SetQuaternion(const float& qx, const float& qy, const float& qz, const float& qw)
		{
			x = qx;
			y = qy;
			z = qz;
			w = qw;
		}

		void SetAngleAxis(const float& angle, const Vector3& axis)
		{
			float theta = angle * static_cast<float>(0.5);
			float s = static_cast<float>(Sin(theta) / Length(axis));
			float c = static_cast<float>(Cos(theta));
	
			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s;
			w = c;
		}

		void GetAngleAxis(float& angle, Vector3& axis) const
		{
			float s = 1 / static_cast<float>(Sqrt(1 - w*w));

			angle = static_cast<float>(ACos(w) * 2.0);
			axis.x = x * s;
			axis.y = y * s;
			axis.z = z * s;
		}

		// get normalized direction vector represented by the Quaternion
		Vector3 GetDirection() const
		{
			float x2 = x+x;
			float y2 = y+y;
			return Vector3(z*x2 + w*y2, y*(z + z) - w*x2, 1.0f-(x*x2 + y*y2));
		}

		float Norm() const
		{
			return x*x + y*y + z*z + w*w;
		}

		float Mod() const
		{
			return static_cast<float>(Sqrt(x*x + y*y + z*z + w*w));
		}

		void Identity()
		{
			x = y = z = 0;
			w = 1;
		}

		void Normalize()
		{
			float s = Norm();
			if ( s == 0 )
			{
				x = y = z = 0;
				w = 1;
			}
			else
			{
				float is = 1 / static_cast<float>(Sqrt(s));
				x *= is;
				y *= is;
				z *= is;
				w *= is;
			}
		}

		void Conjugate()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		void Inverse()
		{
			float n = -1 / Norm();
			x *= n;
			y *= n;
			z *= n;
			w *= -n;
		}

		void Negate()
		{
			float nim = -1 / Mod();
			x *= nim;
			y *= nim;
			z *= nim;
			w *= -nim;
		}

		void Exp()
		{
			float s = static_cast<float>(Sqrt(x*x + y*y + z*z));
			w = static_cast<float>(Cos(s));
			if ( s > EPSILON * 100 )
			{
				s = static_cast<float>(Sin(s)) / s;
				x *= s;
				y *= s;
				z *= s;
			}
		}

		void Log()
		{
			float s = w ? static_cast<float>(ATan(Sqrt(x*x + y*y + z*z),w)) : static_cast<float>(PI * 2);
			x *= s;
			y *= s;
			z *= s;
			w = 0;
		}

		void LnDif(const Quaternion& q)
		{
			Quaternion invp = *this;
			invp.Inverse();
			Quaternion mulp = invp * q;
	
			float length = static_cast<float>(Sqrt(mulp.x*mulp.x + mulp.y*mulp.y + mulp.z*mulp.z));
			float scale = x*x + y*y + z*z + w*w;
			float mval = scale ? static_cast<float>(ATan(length,scale)) : static_cast<float>(PI * 2);
			if ( length != 0 ) mval /= length;
	
			x = mulp.x * mval;
			y = mulp.y * mval;
			z = mulp.z * mval;
			w = 0;
		}

	};

	// inline functions

	
	inline float DotProduct(const Quaternion& a, const Quaternion& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}

	
	inline Quaternion Lerp(const Quaternion& a, const Quaternion& b, const float& time)
	{
		return Quaternion(
			a.x + (b.x - a.x) * time,
			a.y + (b.y - a.y) * time,
			a.z + (b.z - a.z) * time,
			a.w + (b.w - a.w) * time);
	}

	
	inline Quaternion Slerp(const Quaternion& a, const Quaternion& b, const float& time)
	{

		// ====================================================
		// AART - Advanced Animation and Rendering Techniques
		// ====================================================

		float cosom = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;

		if ( (1 + cosom) > EPSILON )
		{
			float sp;
			float sq;

			if ( (1 - cosom) > EPSILON )
			{
				float omega = ACos(cosom);
				float sinom = 1.0f / Sin(omega);

				sp = static_cast<float>(Sin((1 - time) * omega) * sinom);
				sq = static_cast<float>(Sin(time * omega) * sinom);
			}
			else
			{
				sp = 1 - time;
				sq = time;
			}

			return Quaternion(
				a.x*sp + b.x*sq,
				a.y*sp + b.y*sq,
				a.z*sp + b.z*sq,
				a.w*sp + b.w*sq);
		}
		else
		{
			float halfpi = static_cast<float>(PI / 2);
			float sp = static_cast<float>(Sin((1 - time) * halfpi));
			float sq = static_cast<float>(Sin(time * halfpi));

			return Quaternion(
				a.x*sp - a.y*sq,
				a.y*sp + a.x*sq,
				a.z*sp - a.w*sq,
				a.z);
		}
	}

	
	inline Quaternion Slerp(const Quaternion& a, const Quaternion& b, const float& time, int spin)
	{
		float bflip = 1;
		float tcos = DotProduct(a,b);
	
		if ( tcos < 0 )
		{
			tcos = -tcos;
			bflip = -1;
		}
		//else if
		if ( (1 - tcos) < EPSILON * 100 )
		{
			// linear interpolate
			return a * (1 - time) + b * (time * bflip);
		}
	
		float theta = static_cast<float>(ACos(tcos));
		float phi = theta + static_cast<float>(spin * PI);
		float tsin = static_cast<float>(Sin(theta));
		float beta = static_cast<float>(Sin(theta - time*phi)) / tsin;
		float alpha = static_cast<float>(Sin(time*phi)) / tsin * bflip;
	
		return a * beta + b * alpha;
	}

	
	inline Quaternion Squad(const Quaternion& p, const Quaternion& a, const Quaternion& b, const Quaternion& q, const float& time)
	{
		Quaternion qa = Slerp(p,q,time,0);
		Quaternion qb = Slerp(a,b,time,0);
		float qtime = 2 * time * (1 - time);
		return Slerp(qa,qb,qtime,0);
	}

	void QuaternionToMatrix(const Quaternion& q, Matrix44& m);

} // namespace math

typedef math::Quaternion quat4f;

#endif