#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/OpenGLApp.h"
#include "../include/ShaderProgram.h"
#include "../include/Grid.h"
#include "../include/Point.h"
#include "../include/ShaderProgram.h"
#include "../include/Square.h"

// Shader sources
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.5, 1.0); // Color
}
)";
// OpenGLApp class
void OpenGLApp::run()
{
    initializeOpenGL();
    GLFWwindow *window = createWindow(800, 600, "OpenGL Example");

    glewInit();

    ShaderProgram shaderProgram(vertexShaderSource, fragmentShaderSource);
    shaderProgram.use();

    Grid grid(10, 0.1f);
    Point point(0.0f, 0.0f);
    Square square;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        grid.render();
        point.render(10.0f);
        // square.render(); // Uncomment to render the square

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void OpenGLApp::initializeOpenGL()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow * OpenGLApp::createWindow(int width, int height, const char *title)
{
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window!" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    return window;
}