#ifndef OPENGLAPP_H
#define OPENGLAPP_H

#include <GLFW/glfw3.h>

class OpenGLApp
{
public:
    void run();

private:
    void initializeOpenGL();
    GLFWwindow *createWindow(int width, int height, const char *title);
};

#endif