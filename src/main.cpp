#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

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

// Function declarations
void initializeOpenGL();
GLFWwindow *createWindow(int width, int height, const char *title);
unsigned int compileShader(GLenum type, const char *source);
unsigned int createShaderProgram(const char *vertexSource, const char *fragmentSource);
void initializeBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO, 
                       const float *vertices, unsigned int vertexSize, 
                       const unsigned int *indices, unsigned int indexSize);
void initializeGrid(unsigned int &VAO, unsigned int &VBO, int numLines, float spacing);
void renderGrid(unsigned int VAO, int numLines);
void renderPoint(unsigned int VAO, float size);
void renderSquare(unsigned int VAO, unsigned int EBO);

int main()
{
    // Initialize OpenGL and create a window
    initializeOpenGL();
    GLFWwindow *window = createWindow(800, 600, "OpenGL Example");

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    // Initialize shader program
    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    // Dot (point) data
    float dotVertices[] = {0.0f, 0.0f, 0.0f};
    unsigned int dotIndices[] = {0};
    unsigned int dotVAO, dotVBO, dotEBO;
    initializeBuffers(dotVAO, dotVBO, dotEBO, dotVertices, sizeof(dotVertices), dotIndices, sizeof(dotIndices));

    // Square data
    float squareVertices[] = {
        -0.05f, -0.05f, 0.0f,
         0.05f, -0.05f, 0.0f,
         0.05f,  0.05f, 0.0f,
        -0.05f,  0.05f, 0.0f};
    unsigned int squareIndices[] = {0, 1, 2, 0, 2, 3};
    unsigned int squareVAO, squareVBO, squareEBO;
    initializeBuffers(squareVAO, squareVBO, squareEBO, squareVertices, sizeof(squareVertices), squareIndices, sizeof(squareIndices));

    // Grid data
    unsigned int gridVAO, gridVBO;
    int numLines = 10;
    float spacing = 0.1f;
    initializeGrid(gridVAO, gridVBO, numLines, spacing);

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render grid
        renderGrid(gridVAO, numLines);

        // Render dot
        renderPoint(dotVAO, 10.0f);

        // Render square (uncomment to display)
        // renderSquare(squareVAO, squareEBO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteVertexArrays(1, &dotVAO);
    glDeleteBuffers(1, &dotVBO);
    glDeleteBuffers(1, &dotEBO);
    glDeleteVertexArrays(1, &squareVAO);
    glDeleteBuffers(1, &squareVBO);
    glDeleteBuffers(1, &squareEBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// Function definitions
void initializeOpenGL()
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

GLFWwindow *createWindow(int width, int height, const char *title)
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

unsigned int compileShader(GLenum type, const char *source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(-1);
    }
    return shader;
}

unsigned int createShaderProgram(const char *vertexSource, const char *fragmentSource)
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void initializeBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
                       const float *vertices, unsigned int vertexSize,
                       const unsigned int *indices, unsigned int indexSize)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void initializeGrid(unsigned int &VAO, unsigned int &VBO, int numLines, float spacing)
{
    std::vector<float> vertices;
    for (int i = -numLines; i <= numLines; ++i)
    {
        float pos = i * spacing;
        vertices.insert(vertices.end(), {-1.0f, pos, 0.0f, 1.0f, pos, 0.0f});
        vertices.insert(vertices.end(), {pos, -1.0f, 0.0f, pos, 1.0f, 0.0f});
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void renderGrid(unsigned int VAO, int numLines)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, (numLines * 2 + 1) * 4);
}

void renderPoint(unsigned int VAO, float size)
{
    glPointSize(size);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
}

void renderSquare(unsigned int VAO, unsigned int EBO)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}