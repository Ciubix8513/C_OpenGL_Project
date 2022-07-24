#ifndef _GLOBAL_H_
#define _GLOBAL_H_

typedef struct Camera
{
	vec3 position;
	vec3 rotation;
	float FOV;
	float ScreenNear;
	float ScreenFar;
}Camera;

#define UINT unsigned int

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
extern unsigned int* ind;



#endif