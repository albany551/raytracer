
#include <math/quaternion.hpp>

using namespace math;

void math::QuaternionToMatrix(const Quaternion& q, Matrix44& m)
{
	float	xs, ys, zs;
	float	wx, wy, wz;
	float	xx, xy, xz;
	float	yy, yz, zz;

	xs = q.x + q.x;
	ys = q.y + q.y;
	zs = q.z + q.z;

	wx = q.w * xs;
	wy = q.w * ys;
	wz = q.w * zs;

	xx = q.x * xs;
	xy = q.x * ys;
	xz = q.x * zs;

	yy = q.y * ys;
	yz = q.y * zs;
	zz = q.z * zs;

	m.m44[0][0] = ( 1.0f - ( yy + zz ) );
	m.m44[0][1] = ( xy - wz );
	m.m44[0][2] = ( xz + wy );
	m.m44[0][3] = 0.0f;

	m.m44[1][0] = ( xy + wz );
	m.m44[1][1] = ( 1.0F - ( xx + zz ) );
	m.m44[1][2] = ( yz - wx );
	m.m44[1][3] = 0.0f;

	m.m44[2][0] = ( xz - wy );
	m.m44[2][1] = ( yz + wx );
	m.m44[2][2] = ( 1.0f - ( xx + yy ) );
	m.m44[2][3] = 0.0f;

	m.m44[3][0] = 0.0f;
	m.m44[3][1] = 0.0f;
	m.m44[3][2] = 0.0f;
	m.m44[3][3] = 1.0f;
}

