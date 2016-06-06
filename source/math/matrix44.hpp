
#ifndef MATH_MATRIX44_HPP
#define MATH_MATRIX44_HPP

#include <math/common.hpp>
#include <math/vector3.hpp>

namespace math
{
	struct Matrix44
	{
		/*
	    The matrix convention:

		; offsets

        [ 0  1  2  3]
        [ 4  5  6  7]
        [ 8  9  0 11]
        [12 13 14 15]

		; scaling (sx,sy,sz)

        [sx -- -- --]
        [-- sy -- --]
        [-- -- sz --]
        [-- -- -- --]
    
    	; translation (tx,ty,tz)

        [-- -- -- --]
        [-- -- -- --]
        [-- -- -- --]
        [tx ty tz --]
    
    	; rotation

        [xx xy xz --]  x-axis (xx,xy,xz)
        [yx yy yz --]  y-axis (yx,yy,yz)
        [zx zy zz --]  z-axis (zx,zy,zz)
        [-- -- -- --]

		*/

    	union
    	{
    		float m16[16];
    		float m44[4][4];
    	};

		Matrix44()
		{
		}

		Matrix44(const float& s)
		{
			m44[0][0] = s; m44[0][1] = 0; m44[0][2] = 0; m44[0][3] = 0;
			m44[1][0] = 0; m44[1][1] = s; m44[1][2] = 0; m44[1][3] = 0;
			m44[2][0] = 0; m44[2][1] = 0; m44[2][2] = s; m44[2][3] = 0;
			m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
		}

		Matrix44(const Matrix44& u)
		{
			for ( int i=0; i<4; ++i )
			{
				m44[i][0] = u.m44[i][0];
				m44[i][1] = u.m44[i][1];
				m44[i][2] = u.m44[i][2];
				m44[i][3] = u.m44[i][3];
			}
		}

		Matrix44 operator + (const Matrix44& u) const
		{
			Matrix44 r;
			for ( int i=0; i<4; ++i )
			{
				r.m44[i][0] = m44[i][0] + u.m44[i][0];
				r.m44[i][1] = m44[i][1] + u.m44[i][1];
				r.m44[i][2] = m44[i][2] + u.m44[i][2];
				r.m44[i][3] = m44[i][3] + u.m44[i][3];
			}
			return r;
		}

		Matrix44 operator - (const Matrix44& u) const
		{
			Matrix44 r;
			for ( int i=0; i<4; ++i )
			{
				r.m44[i][0] = m44[i][0] - u.m44[i][0];
				r.m44[i][1] = m44[i][1] - u.m44[i][1];
				r.m44[i][2] = m44[i][2] - u.m44[i][2];
				r.m44[i][3] = m44[i][3] - u.m44[i][3];
			}
			return r;
		}

		Matrix44 operator * (const Matrix44& u) const
		{
			Matrix44 r;
			for ( int i=0; i<4; ++i )
			{
				const float* v = m16 + i * 4;
				r.m44[i][0] = v[0]*u.m44[0][0] + v[1]*u.m44[1][0] + v[2]*u.m44[2][0] + v[3]*u.m44[3][0];
				r.m44[i][1] = v[0]*u.m44[0][1] + v[1]*u.m44[1][1] + v[2]*u.m44[2][1] + v[3]*u.m44[3][1];
				r.m44[i][2] = v[0]*u.m44[0][2] + v[1]*u.m44[1][2] + v[2]*u.m44[2][2] + v[3]*u.m44[3][2];
				r.m44[i][3] = v[0]*u.m44[0][3] + v[1]*u.m44[1][3] + v[2]*u.m44[2][3] + v[3]*u.m44[3][3];
			}
			return r;
		}

		Matrix44& operator += (const Matrix44& u)
		{
			for ( int i=0; i<4; ++i )
			{
				m44[i][0] += u.m44[i][0];
				m44[i][1] += u.m44[i][1];
				m44[i][2] += u.m44[i][2];
				m44[i][3] += u.m44[i][3];
			}
			return *this;
		}

		Matrix44& operator -= (const Matrix44& u)
		{
			for ( int i=0; i<4; ++i )
			{
				m44[i][0] -= u.m44[i][0];
				m44[i][1] -= u.m44[i][1];
				m44[i][2] -= u.m44[i][2];
				m44[i][3] -= u.m44[i][3];
			}
			return *this;
		}

		Matrix44& operator *= (const Matrix44& u)
		{
			for ( int i=0; i<4; ++i )
			{
				float v[4];
				v[0] = m44[i][0];
				v[1] = m44[i][1];
				v[2] = m44[i][2];
				v[3] = m44[i][3];
				m44[i][0] = v[0]*u.m44[0][0] + v[1]*u.m44[1][0] + v[2]*u.m44[2][0] + v[3]*u.m44[3][0];
				m44[i][1] = v[0]*u.m44[0][1] + v[1]*u.m44[1][1] + v[2]*u.m44[2][1] + v[3]*u.m44[3][1];
				m44[i][2] = v[0]*u.m44[0][2] + v[1]*u.m44[1][2] + v[2]*u.m44[2][2] + v[3]*u.m44[3][2];
				m44[i][3] = v[0]*u.m44[0][3] + v[1]*u.m44[1][3] + v[2]*u.m44[2][3] + v[3]*u.m44[3][3];
			}
			return *this;
		}

		Matrix44& operator *= (const float& s)
		{
			for ( int i=0; i<16; ++i )
			{
				m16[i] *= s;
			}
			return *this;
		}

		void operator = (const float& s)
		{
			m44[0][0] = s; m44[0][1] = 0; m44[0][2] = 0; m44[0][3] = 0;
			m44[1][0] = 0; m44[1][1] = s; m44[1][2] = 0; m44[1][3] = 0;
			m44[2][0] = 0; m44[2][1] = 0; m44[2][2] = s; m44[2][3] = 0;
			m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
		}

		void operator = (const Matrix44& u)
		{
			for ( int i=0; i<4; ++i )
			{
				m44[i][0] = u.m44[i][0];
				m44[i][1] = u.m44[i][1];
				m44[i][2] = u.m44[i][2];
				m44[i][3] = u.m44[i][3];
			}
		}

		void Identity()
		{
			m44[0][0] = 1; m44[0][1] = 0; m44[0][2] = 0; m44[0][3] = 0;
			m44[1][0] = 0; m44[1][1] = 1; m44[1][2] = 0; m44[1][3] = 0;
			m44[2][0] = 0; m44[2][1] = 0; m44[2][2] = 1; m44[2][3] = 0;
			m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
		}

		void SetScale(const float& x, const float& y, const float& z)
		{
			m44[0][0] = x; m44[0][1] = 0; m44[0][2] = 0; m44[0][3] = 0;
			m44[1][0] = 0; m44[1][1] = y; m44[1][2] = 0; m44[1][3] = 0;
			m44[2][0] = 0; m44[2][1] = 0; m44[2][2] = z; m44[2][3] = 0;
			m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
		}

		void SetScale(const Vector3& scale)
		{
			SetScale(scale.x,scale.y,scale.z);
		}

		void Scale(const Vector3& scale)
		{
			m44[0][0] = scale.x * m44[0][0];
			m44[1][1] = scale.y * m44[1][1];
			m44[2][2] = scale.z * m44[2][2];
		}

		void SetTranslation(const float& x, const float& y, const float z)
		{
			m44[3][0] = x; m44[3][1] = y; m44[3][2] = z; m44[3][3] = 1;
		}

		void SetTranslation(const Vector3& translate)
		{
			SetTranslation(translate.x,translate.y,translate.z);
		}
		
		Vector3 GetTranslation() const
		{
            return Vector3(m44[3][0],m44[3][1],m44[3][2]);
        }
        
        Vector3 GetDirection() const
        {
            return Normalize(Vector3(m44[0][2], m44[1][2], m44[2][2]));
        }

		void Transpose()
		{
			Swap(m44[0][1],m44[1][0]);
			Swap(m44[0][2],m44[2][0]);
			Swap(m44[0][3],m44[3][0]);
			Swap(m44[1][2],m44[2][1]);
			Swap(m44[1][3],m44[3][1]);
			Swap(m44[2][3],m44[3][2]);
		}

		void Transpose(const Matrix44& u)
		{
			m44[0][0] = u.m44[0][0];
			m44[0][1] = u.m44[1][0];
			m44[0][2] = u.m44[2][0];
			m44[0][3] = u.m44[3][0];
			m44[1][0] = u.m44[0][1];
			m44[1][1] = u.m44[1][1];
			m44[1][2] = u.m44[2][1];
			m44[1][3] = u.m44[3][1];
			m44[2][0] = u.m44[0][2];
			m44[2][1] = u.m44[1][2];
			m44[2][2] = u.m44[2][2];
			m44[2][3] = u.m44[3][2];
			m44[3][0] = u.m44[0][3];
			m44[3][1] = u.m44[1][3];
			m44[3][2] = u.m44[2][3];
			m44[3][3] = u.m44[3][3];
        }

		float GetDeterminant() const
		{
				float det;
				det = m16[0] * m16[5] * m16[10];
				det += m16[4] * m16[9] * m16[2];
				det += m16[8] * m16[1] * m16[6];
				det -= m16[8] * m16[5] * m16[2];
				det -= m16[4] * m16[1] * m16[10];
				det -= m16[0] * m16[9] * m16[6];
				return det;
		}

		void SetView(const Vector3& target, const Vector3& source, const Vector3& up);

		void SetPerspective(float fov, float aspect, float zNear, float zFar);
	
		void SetOrthogonal(float left, float right, float bottom, float top, float znear, float zfar);

		Matrix44 Inverse() const;

	};

	inline void operator *= (const Matrix44& m, Vector3& v)
	{
		v =	Vector3(
			v.x*m.m44[0][0] + v.y*m.m44[1][0] + v.z*m.m44[2][0] + m.m44[3][0],
			v.x*m.m44[0][1] + v.y*m.m44[1][1] + v.z*m.m44[2][1] + m.m44[3][1],
			v.x*m.m44[0][2] + v.y*m.m44[1][2] + v.z*m.m44[2][2] + m.m44[3][2]);
	}

	inline Vector3 operator * (const Matrix44& m, const Vector3& v)
	{
		return Vector3(
			v.x*m.m44[0][0] + v.y*m.m44[1][0] + v.z*m.m44[2][0] + m.m44[3][0],
			v.x*m.m44[0][1] + v.y*m.m44[1][1] + v.z*m.m44[2][1] + m.m44[3][1],
			v.x*m.m44[0][2] + v.y*m.m44[1][2] + v.z*m.m44[2][2] + m.m44[3][2]);
	}
	
} // namespace math

typedef math::Matrix44 matrix44f;

#endif
