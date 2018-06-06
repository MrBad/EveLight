#include "gl_program.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace evl {

GLProgram::GLProgram()
{
    mProgram = mVertexShader = mFragmentShader = mNumAttributes = 0;
}

bool GLProgram::Create(const std::string& shadersPath)
{
    mProgram = glCreateProgram();
    mVertexShader = glCreateShader(GL_VERTEX_SHADER);
    mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    mNumAttributes = 0;
    if (!CompileShaders(shadersPath))
        return false;
    if (!LinkShaders())
        return false;
    Use();

    return true;
}

GLProgram::~GLProgram()
{
    glUseProgram(0);

    glDetachShader(mProgram, mVertexShader);
    glDetachShader(mProgram, mFragmentShader);

    glDeleteShader(mVertexShader);
    glDeleteShader(mFragmentShader);

    glDeleteProgram(mProgram);
}

static bool fileGet(const std::string& path, std::string& content)
{
    content.clear();
    std::string line;

    std::ifstream file(path);
    if (file.fail()) {
        std::cerr << "Cannot open file: " << path << std::endl;
        return false;
    }
    while (getline(file, line))
        content += line + '\n';
    file.close();

    return true;
}

bool GLProgram::CompileShader(const std::string& shaderCode, GLuint shaderID,
    const std::string& name)
{
    const char* ptr = shaderCode.c_str();
    glShaderSource(shaderID, 1, &ptr, nullptr);
    glCompileShader(shaderID);

    GLint success = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint maxLen;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLen);

        std::vector<char> logMessage(maxLen);
        glGetShaderInfoLog(shaderID, maxLen, &maxLen, &logMessage[0]);

        glDeleteShader(shaderID);
        std::cerr << "Error in shader: " << name << std::endl;
        std::cerr << &logMessage[0] << std::endl;
        return false;
    }

    return true;
}

bool GLProgram::LinkShaders()
{
    glAttachShader(mProgram, mVertexShader);
    glAttachShader(mProgram, mFragmentShader);
    glLinkProgram(mProgram);

    GLint success = GL_FALSE;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLint maxLen;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &maxLen);
        std::vector<char> logMessage(maxLen);
        glGetProgramInfoLog(mProgram, maxLen, &maxLen, &logMessage[0]);

        glDeleteProgram(mProgram);
        glDetachShader(mProgram, mVertexShader);
        glDetachShader(mProgram, mFragmentShader);
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);

        std::cerr << "Error in linking shaders!" << std::endl;
        std::cerr << &logMessage[0] << std::endl;
        return false;
    }

    return true;
}

bool GLProgram::CompileShaders(const std::string& shadersPath)
{
    std::string shaderContent;

    if (!fileGet(shadersPath + ".vs", shaderContent))
        return false;
    if (!CompileShader(shaderContent, mVertexShader, shadersPath + ".vs"))
        return false;
    if (!fileGet(shadersPath + ".fs", shaderContent))
        return false;
    if (!CompileShader(shaderContent, mFragmentShader, shadersPath + ".fs"))
        return false;

    return true;
}

void GLProgram::addAttribute(const std::string& name)
{
    glBindAttribLocation(mProgram, mNumAttributes++, name.c_str());
}

GLint GLProgram::getAttribute(const std::string& name)
{
    return glGetAttribLocation(mProgram, name.c_str());
}

GLuint GLProgram::getUniformLocation(const std::string& uniformName)
{
    GLint location = glGetUniformLocation(mProgram, uniformName.c_str());
    if ((unsigned)location == GL_INVALID_INDEX) {
        std::cerr << "; Uniform: " << uniformName
                  << " not found in shader for program " << mProgram
                  << std::endl;
        return 0;
    }

    return location;
}

void GLProgram::Use()
{
    glUseProgram(mProgram);
}

void GLProgram::Unuse()
{
    glUseProgram(0);
}
}
