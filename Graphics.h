#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "FileLoader.h"
#include "global.h"
#include <time.h>
// extern GLFWwindow *wnd;
// extern UINT VBO;
// extern UINT VAO;
// extern UINT EBO;
// extern UINT texture;
// extern UINT ind_size;
// extern long long prevTime;
// extern double dT;
// extern long long startT;
// extern mat4 proj, camera;
// extern vec2 res;
// extern Camera cam;
// extern UINT ShaderProg;
// extern unsigned int* ind;
//Shader s;
int InitWindow(int height, int width,char * title);
int InitOpenGL();

void setUpModel();
int setupShaders();
int setupTexture();
void Render();

void Cleanup();

uint32_t getTick();

#endif 
