#include "System.h"

int Start()
{
	int res = Init();

	if (res != 1)
	{
		printf("Failed init with, %i \n", res);
		return res;
	}	
	Run();

	return 1;
}

int Init()
{

	InitWindow(720, 1080, "C OpenGL test");
	glfwSetWindowSizeCallback(wnd, WindowResizeCallback);
	glfwSetKeyCallback(wnd,KeyCallback);
	glfwSetCursorPosCallback(wnd,CursorPosCallback);
	glfwSetMouseButtonCallback(wnd,MouseButtonCallback);
	return InitOpenGL();
	return 1;
}
int Run()
{
	Frame();

	Exit();
	return 1;
}
void Frame()
{
	printf("First frame\n");
	while (!glfwWindowShouldClose(wnd))
	{
		ResetKeyUp();
		ResetMouseUp();
		Render();		
		glfwSwapBuffers(wnd);
		glfwPollEvents();
		if(Input.Mouse.cursorLocked)
			glfwSetCursorPos(wnd,res.x / 2,res.y / 2);
	}
}

void Exit()
{
	Cleanup();
}

void WindowResizeCallback(GLFWwindow *window,int x,int y)
{	
	glViewport(0, 0, x, y);
}