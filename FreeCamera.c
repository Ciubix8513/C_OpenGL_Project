#include "FreeCamera.h"
Camera FreeCam;

void SetupCamera()
{
    FreeCam.FOV = 60;
    FreeCam.ScreenFar = 1000;
    FreeCam.ScreenNear = 0.1f;
    FreeCam.position = Vec3(0, 0, -5);
    FreeCam.rotation = Vec3(0, 0, 0);
}

void UpdateCamera()
{
    if (Input.Mouse.cursorLocked)
    {
        float mouseSens = 0.05f;
        FreeCam.rotation.x += Input.Mouse.Delta.y * mouseSens;
        FreeCam.rotation.y += Input.Mouse.Delta.x * mouseSens;
        vec4 Ivec = Vec4(0, 0, 0, 1);
        float speed = 0.02f;
        if(glfwGetKey(wnd, GLFW_KEY_LEFT_SHIFT))
        speed *=2;
        if (glfwGetKey(wnd, GLFW_KEY_W))
            Ivec.z += speed;
        if (glfwGetKey(wnd, GLFW_KEY_S))
            Ivec.z -= speed;
        if (glfwGetKey(wnd, GLFW_KEY_D))
            Ivec.x += speed;
        if (glfwGetKey(wnd, GLFW_KEY_A))
            Ivec.x -= speed;
        if (glfwGetKey(wnd, GLFW_KEY_E))
            Ivec.y += speed;
        if (glfwGetKey(wnd, GLFW_KEY_Q))
            Ivec.y -= speed;
        Ivec = MultMatVec(Ivec, RotMatrix(FreeCam.rotation));
        FreeCam.position.x += Ivec.x;
        FreeCam.position.y += Ivec.y;
        FreeCam.position.z += Ivec.z;
    }
}