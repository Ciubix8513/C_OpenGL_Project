#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <GLFW/glfw3.h>

#define UINT unsigned int
// I know this is probably some shitty design but idk

#pragma pack(push, 1)
typedef struct vec3
{
	float x, y, z;
	// char c ;
} vec3;
typedef struct vec3i
{
	int x, y, z;
	// char c;
} vec3i;
typedef struct vec2
{
	float x, y;
	// char c;
} vec2;
typedef struct vec4
{
	float x, y, z, w;
} vec4;
typedef struct mat4
{
	// vec4 m0, m1, m2, m3;
	float
		m00,
		m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33;
} mat4;
#pragma pack(pop)

#define PI 3.14159265f
#define deg2rad PI / 180.0f


#pragma region _INPUT_H_
typedef struct S_Mouse
{
	char Up[GLFW_MOUSE_BUTTON_LAST + 1];
	char Down[GLFW_MOUSE_BUTTON_LAST + 1];
	char cursorLocked;
	int toReset[GLFW_MOUSE_BUTTON_LAST + 1];
	int resetKeys;
	vec2 prevPos;
	vec2 Pos;
	vec2 Delta;

	int XPos,YPos;
} S_Mouse;

typedef struct S_Input
{
	char KeyDown[GLFW_KEY_LAST + 1];
	char KeyPressed[GLFW_KEY_LAST + 1];
	char KeyUp[GLFW_KEY_LAST + 1];
	int toReset[GLFW_KEY_LAST + 1];
	int resetKeys;
	S_Mouse Mouse;
} S_Input;
extern S_Input Input;
#pragma endregion
#pragma region _GRPAPHICS_H_
typedef struct Camera
{
	vec3 position;
	vec3 rotation;
	float FOV;
	float ScreenNear;
	float ScreenFar;
} Camera;

extern Camera FreeCam;

extern GLFWwindow *wnd;
extern UINT VBO;
extern UINT VAO;
extern UINT EBO;
extern UINT texture;
extern UINT ind_size;
extern long long prevTime;
extern double dT;
extern long long startT;
extern mat4 proj, camera;
extern vec2 res;
extern Camera cam;
extern UINT ShaderProg;
extern unsigned int *ind;
#pragma endregion
#endif