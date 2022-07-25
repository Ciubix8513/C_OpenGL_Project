#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#include "Graphics.h"
#include "Input.h"
int Start();

int Init();
int Run();

void Frame();
void Exit();

//GLFWwindowsizefun WindowResizeCallback(GLFWwindow* window,GLFWwindowsizefun cbfun);
void WindowResizeCallback(GLFWwindow *window,int x,int y);

#endif
