#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#include "Graphics.h"
int Start();

int Init();
int Run();

void Frame();
void Exit();

GLFWwindowsizefun WindowResizeCallback(GLFWwindow* window,GLFWwindowsizefun cbfun);
#endif
