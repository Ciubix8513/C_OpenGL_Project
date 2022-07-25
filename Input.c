#include "Input.h"

S_Input Input;

void KeyCallback(GLFWwindow *wnd, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        Input.Mouse.cursorLocked = 0;
        glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    switch (action)
    {
    case GLFW_PRESS:
        Input.KeyDown[key] = 1;
        break;
    case GLFW_REPEAT:
        Input.KeyDown[key] = 0;
        Input.KeyPressed[key] = 1;
        break;
    case GLFW_RELEASE:
        Input.KeyPressed[key] = 0;
        Input.KeyUp[key] = 1;
        Input.toReset[Input.resetKeys] = key;
        Input.resetKeys++;
        break;
    }
}

void CursorPosCallback(GLFWwindow *wnd, double xPos, double yPos)
{    
    Input.Mouse.prevPos = Input.Mouse.Pos;
    Input.Mouse.Pos.x = xPos;
    Input.Mouse.Pos.y = yPos;
    Input.Mouse.Delta.x = Input.Mouse.Pos.x - Input.Mouse.prevPos.x;
    Input.Mouse.Delta.y = Input.Mouse.Pos.y - Input.Mouse.prevPos.y;
}

void MouseButtonCallback(GLFWwindow *wnd, int button, int action, int mode)
{
    if (button == 0)
    {
        Input.Mouse.cursorLocked = 1;
        glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    
    if (action == GLFW_PRESS)
    {
        Input.Mouse.Down[button] = 1;
    }
    else
    {
        Input.Mouse.Down[button] = 0;
        Input.Mouse.Up[button] = 1;
        Input.Mouse.toReset[Input.Mouse.resetKeys] = button;
        Input.Mouse.resetKeys++;
    }
}

void ResetKeyUp()
{
    for (size_t i = 0; i < Input.resetKeys; i++)
        Input.KeyUp[Input.toReset[i]];
    Input.resetKeys = 0;
}
void ResetMouseUp()
{
    for (size_t i = 0; i < Input.Mouse.resetKeys; i++)
        Input.Mouse.Up[Input.Mouse.toReset[i]];
    Input.Mouse.resetKeys = 0;
}
