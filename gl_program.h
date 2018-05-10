#pragma once

#include <GL/glew.h>
#include <string>

class GLProgram {
public:
    GLProgram();
    ~GLProgram();
    bool Create(const std::string& shadersPath);
    void addAttribute(const std::string& name);
    GLint getAttribute(const std::string& name);
    GLuint getId() { return mProgram; }
    GLuint getUniformLocation(const std::string& uniformName);
    void Use();
    void Unuse();

private:
    GLuint mProgram;
    GLuint mVertexShader;
    GLuint mFragmentShader;
    GLuint mNumAttributes;
    bool CompileShader(const std::string& shaderCode, GLuint shaderID, const std::string& name);
    bool CompileShaders(const std::string& shadersPath);
    bool LinkShaders();
};
