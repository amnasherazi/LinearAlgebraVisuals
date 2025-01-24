#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Vertex shader source
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment shader source
const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.5, 1.0); // Grid line color (gray)
}
)";

// Function to initialize buffers for rendering
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO
}

// Function to initialize a grid
void initializeGrid(unsigned int &VAO, unsigned int &VBO, int numLines, float spacing)
{
    std::vector<float> vertices;

    // Horizontal lines
    for (int i = -numLines; i <= numLines; ++i)
    {
        float y = i * spacing;
        vertices.push_back(-1.0f);
        vertices.push_back(y);
        vertices.push_back(0.0f); // Start
        vertices.push_back(1.0f);
        vertices.push_back(y);
        vertices.push_back(0.0f); // End
    }

    // Vertical lines
    for (int i = -numLines; i <= numLines; ++i)
    {
        float x = i * spacing;
        vertices.push_back(x);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f); // Start
        vertices.push_back(x);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f); // End
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
}

// Function to compile a shader and check for errors
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
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        exit(-1);
    }

    return shader;
}

// Function to create and link a shader program
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
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Example", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    // Dot (point) data
    float dotVertices[] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    unsigned int dotIndices[] = {0};

    // Square data
    float squareVertices[] = {
        -0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.05f, 0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.05f, 0.05f, 0.0f, 1.0f, 0.0f, 0.0f};
    unsigned int squareIndices[] = {0, 1, 2, 0, 2, 3};

    // Initialize buffers
    unsigned int dotVAO, dotVBO, dotEBO;
    initializeBuffers(dotVAO, dotVBO, dotEBO, dotVertices, sizeof(dotVertices), dotIndices, sizeof(dotIndices));

    unsigned int squareVAO, squareVBO, squareEBO;
    initializeBuffers(squareVAO, squareVBO, squareEBO, squareVertices, sizeof(squareVertices), squareIndices, sizeof(squareIndices));

    unsigned int gridVAO, gridVBO;
    int numLines = 10;    // Number of lines in each direction
    float spacing = 0.1f; // Spacing between lines

    initializeGrid(gridVAO, gridVBO, numLines, spacing);

    // Create shader program
    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(gridVAO);
        glDrawArrays(GL_LINES, 0, (numLines * 2 + 1) * 4);

        // Draw the dot
        glPointSize(10.0f);
        glBindVertexArray(dotVAO);
        glDrawArrays(GL_POINTS, 0, 1);

        // Draw the square
        // glBindVertexArray(squareVAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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