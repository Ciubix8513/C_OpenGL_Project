#include "Math.h"

vec3 Vec3(float a, float b, float c)
{
	vec3 v;
	v.x = a;
	v.y = b;
	v.z = c;

	return v;
}

float LenV3(vec3 v)
{
	return (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 NormalizedV3(vec3 v)
{
	float l = LenV3(v);
	return Vec3(v.x / l, v.y / l, v.z / l);
}

vec3 subV3(vec3 a, vec3 b)
{
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 addV3(vec3 a, vec3 b)
{
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 cross(vec3 a, vec3 b)
{
	return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float dotV3(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3i Vec3i(int a, int b, int c)
{
	vec3i v;
	v.x = a;
	v.y = b;
	v.z = c;

	return v;
}

vec2 Vec2(int a, int b)
{
	vec2 v;
	v.x = a;
	v.y = b;

	return v;
}

vec4 Vec4(float a, float b, float c, float d)
{
	vec4 v;
	v.x = a;
	v.y = b;
	v.z = c;
	v.w = d;
	return v;
}

mat4 Identity()
{
	return Mat4m(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);
}

mat4 Mat4(vec4 a, vec4 b, vec4 c, vec4 d)
{
	mat4 m;
	m.m0 = a;
	m.m1 = b;
	m.m2 = c;
	m.m3 = d;
	return m;
}

mat4 Mat4m(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	mat4 m;
	m.m0 = Vec4(m00, m01, m02, m03);
	m.m1 = Vec4(m10, m11, m12, m13);
	m.m2 = Vec4(m20, m21, m22, m23);
	m.m3 = Vec4(m30, m31, m32, m33);
	return m;
}

mat4 PerspectiveProj(float FOV, float Screen, float near, float far)
{
	float xScale, yScale, C, D;
	yScale = 1.0f / (float)tan((FOV * deg2rad) / 2.0f);
	xScale = yScale / Screen;

	C = far / (far - near);
	D = -near * far / (far - near);

	return Mat4m(
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, C, 1,
		0, 0, D, 0);
}

mat4 LookAt(vec3 eye, vec3 at, vec3 up)
{
	vec3 z, x, y;
	z = NormalizedV3(subV3(at, eye));
	x = NormalizedV3(cross(z, up));
	y = cross(x, z);

	return Mat4m(
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		-dotV3(x, eye), -dotV3(y, eye), -dotV3(z, eye), 1);
	/*return Mat4m(
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		-x.x, -x.y, -x.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);*/
}