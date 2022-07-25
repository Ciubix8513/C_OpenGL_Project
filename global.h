#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include "Math.h"
#include <GLFW/glfw3.h>

#define UINT unsigned int
// I know this is probably some shitty design but idk
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