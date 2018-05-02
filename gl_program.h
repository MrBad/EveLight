#pragma once

#include <iostream>
#include <GL/glew.h>

using namespace std;

class GLProgram
{
  public:
    GLProgram();
    ~GLProgram();
    bool Create(const string &shadersPath);
    void addAttribute(const string &name);
    GLint getAttribute(const string &name);
    GLuint getId() { return mProgram; }
    GLuint getUniformLocation(const std::string& uniformName);
    void Use();
    void Unuse();

  private:
    GLuint mProgram;
    GLuint mVertexShader;
    GLuint mFragmentShader;
    GLuint mNumAttributes;
    bool CompileShader(const string &shaderCode, GLuint shaderID, const string &name);
    bool CompileShaders(const string &shadersPath);
    bool LinkShaders();
};
