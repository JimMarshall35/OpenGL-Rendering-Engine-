#pragma once
// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
#include <iostream>
#include "Cameras.h"
static bool wantMouseInput = true;
static bool wantKeyboardInput = true;
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!wantMouseInput) {
        static double lastx = 0;
        static double lasty = 0;

        double dx = xpos - lastx;
        double dy = ypos - lasty;

        lastx = xpos;
        lasty = ypos;
        if (glfwGetMouseButton(window, 0) == GLFW_PRESS) {
            Cameras::instance().getActive()->ProcessMouseMovement(dx, dy);
        }

        //std::cout << dx << " " << dy << std::endl;
    }
    
}
static void processInput(GLFWwindow *window, float delta)
{
    if (!wantKeyboardInput){
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Cameras::instance().getActive()->ProcessKeyboard(FORWARD, delta);
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Cameras::instance().getActive()->ProcessKeyboard(BACKWARD, delta);
        }
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Cameras::instance().getActive()->ProcessKeyboard(LEFT, delta);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Cameras::instance().getActive()->ProcessKeyboard(RIGHT, delta);
        }
    }
    
    

}
static void inputInit(GLFWwindow *window) {
    glfwSetCursorPosCallback(window, cursor_position_callback);
}
