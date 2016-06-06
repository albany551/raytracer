
#include <math/matrix44.hpp>

using namespace math;

void Matrix44::SetView(const Vector3& target, const Vector3& source, const Vector3& up)
{
	Vector3 z = Normalize(target - source);
	Vector3 x = Normalize(CrossProduct(up,z));
	Vector3 y = CrossProduct(z,x);

	m44[0][0] = x.x;
	m44[0][1] = y.x;
	m44[0][2] = z.x;
	m44[0][3] = 0;
	m44[1][0] = x.y;
	m44[1][1] = y.y;
	m44[1][2] = z.y;
	m44[1][3] = 0;
	m44[2][0] = x.z;
	m44[2][1] = y.z;
	m44[2][2] = z.z;
	m44[2][3] = 0;
	m44[3][0] = 0; //DotProduct(-x, source);
	m44[3][1] = 0; //DotProduct(-y, source);
	m44[3][2] = 0; //DotProduct(-z, source);
	m44[3][3] = 1;
}


void Matrix44::SetPerspective(float fov, float aspect, float zNear, float zFar)
{
	float radians = fov / 2 * PI / 180;
	float deltaZ = zFar - zNear;
	float sine = Sin(radians);
	if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) return;
	float cotangent = Cos(radians) / sine;

	m44[0][0] = cotangent / aspect;
	m44[0][1] = 0;
	m44[0][2] = 0;
	m44[0][3] = 0;
	m44[1][0] = 0;
	m44[1][1] = cotangent;
	m44[1][2] = 0;
	m44[1][3] = 0;
	m44[2][0] = 0;
	m44[2][1] = 0;
	m44[2][2] = -(zFar + zNear) / deltaZ;
	m44[2][3] = -1;
	m44[3][0] = 0;
	m44[3][1] = 0;
	m44[3][2] = -2 * zNear * zFar / deltaZ;
	m44[3][3] = 0;

}

void Matrix44::SetOrthogonal(float left, float right, float bottom, float top, float znear, float zfar)
{
	float rl = right - left;
	float t1 = top - bottom;
	float f1 = zfar - znear;

	m44[0][0] = 2 / rl;				m44[0][1] = 0;					m44[0][2] = 0;					m44[0][3] = 0;
	m44[1][0] = 0;					m44[1][1] = 2 / t1;				m44[1][2] = 0;					m44[1][3] = 0;
	m44[2][0] = 0;					m44[2][1] = 0;					m44[2][2] = -2 / f1;			m44[2][3] = 0;
	m44[3][0] = -(right+left)/rl;	m44[3][1] = -(top+bottom)/t1;	m44[3][2] = -(zfar+znear)/f1;	m44[3][3] = 1;
}

// FIXME: not a correct inverse!
// here's where your transform problem (vs. raytracer) happens...
Matrix44 Matrix44::Inverse() const
{
	Matrix44 ret;
	Vector3 translation = -GetTranslation();
	Vector3 direction = -GetDirection();
	ret.SetView(translation + direction, translation, Vector3(0,1,0));
	ret.SetTranslation(translation);
	return ret;
}


