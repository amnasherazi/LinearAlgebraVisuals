#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>

class ShaderProgram {
public:
    ShaderProgram(const char *vertexSource, const char *fragmentSource);
    void use() const;
    ~ShaderProgram();

private:
    unsigned int programID;
    unsigned int compileShader(GLenum type, const char *source);
    void checkLinkingErrors();
};

#endif