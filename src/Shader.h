#pragma once
#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader() = default;
    Shader(const char* vertSrc, const char* fragSrc);
    ~Shader();
    void use() const;
    GLuint id() const { return program; }
private:
    GLuint program = 0;
    GLuint compile(GLuint type, const char* src);
};
