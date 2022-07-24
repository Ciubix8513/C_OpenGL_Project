#ifndef _MATH_H_
#define _MATH_H_
#include <math.h>

#pragma pack(push,1)
typedef struct vec3
{
	float x, y, z;
	//char c ;
}vec3;
typedef struct vec3i
{
	int x, y, z;
//char c;
}vec3i;
typedef struct vec2
{
	float x, y;
	//char c;
} vec2;
typedef struct vec4
{
	float x, y, z, w;
}vec4;
typedef struct mat4
{
	vec4 m0, m1, m2, m3;
}mat4;
#pragma pack(pop)

#define PI 3.14159265f
#define deg2rad PI / 180.0f

vec3 Vec3(float a, float b, float c);
float LenV3(vec3 v);
vec3 NormalizedV3(vec3 v);
vec3 subV3(vec3 a, vec3 b);
vec3 addV3(vec3 a, vec3 b);
vec3 cross(vec3 a, vec3 b);

float dotV3(vec3 a, vec3 b);

vec3i Vec3i(int a, int b, int c);
vec2 Vec2(int a, int b);

vec4 Vec4(float a, float b, float c, float d);

mat4 Identity();
mat4 Mat4(vec4 a, vec4 b, vec4 c, vec4 d);
mat4 Mat4m(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33);
mat4 PerspectiveProj(float FOV, float Screen, float ScreenNear, float ScreenFar);
mat4 LookAt(vec3 eye, vec3 at, vec3 up);
#endif