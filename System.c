#include "System.h"


int Start()
{
	int res = Init();

	if (res != 1){printf("Failed init with, %i \n",res);return res;}
	glfwSetWindowSizeCallback(wnd,WindowResizeCallback);
	Run();
	
	return 1;
}

int Init()
{
	
	InitWindow(720,1080,"C OpenGL test");
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
	while(!glfwWindowShouldClose(wnd))
	{

		Render();		
		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

}
void Exit()
{
	Cleanup();  
}

GLFWwindowsizefun WindowResizeCallback(GLFWwindow* window,GLFWwindowsizefun cbfun){	
	int x,y;
	glfwGetWindowSize(wnd,&x,&y);
	glViewport(0,0,x,y);
}