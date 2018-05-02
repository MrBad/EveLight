#include <iostream>
#include <fstream>
#include <vector>
#include "gl_program.h"

GLProgram::GLProgram()
{
    mProgram = mVertexShader = mFragmentShader = mNumAttributes = 0;
}

bool GLProgram::Create(const string &shadersPath)
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

static bool fileGet(const string &path, string &content)
{
    content.clear();
    string line;

    ifstream file(path);
    if (file.fail())
    {
        cerr << "Cannot open file: " << path << endl;
        return false;
    }
    while (getline(file, line))
        content += line + '\n';
    file.close();

    return true;
}

bool GLProgram::CompileShader(const string &shaderCode, GLuint shaderID, const string &name)
{
    const char *ptr = shaderCode.c_str();
    glShaderSource(shaderID, 1, &ptr, nullptr);
    glCompileShader(shaderID);

    GLint success = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLen;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLen);

        vector<char> logMessage(maxLen);
        glGetShaderInfoLog(shaderID, maxLen, &maxLen, &logMessage[0]);

        glDeleteShader(shaderID);
        cerr << "Error in shader: " << name << endl;
        cerr << &logMessage[0] << endl;
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
    if (success == GL_FALSE)
    {
        GLint maxLen;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &maxLen);
        std::vector<char> logMessage(maxLen);
        glGetProgramInfoLog(mProgram, maxLen, &maxLen, &logMessage[0]);

        glDeleteProgram(mProgram);
        glDetachShader(mProgram, mVertexShader);
        glDetachShader(mProgram, mFragmentShader);
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);

        cerr << "Error in linking shaders!" << endl;
        cerr << &logMessage[0] << endl;
        return false;
    }

    return true;
}

bool GLProgram::CompileShaders(const std::string &shadersPath)
{
    string shaderContent;

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

void GLProgram::addAttribute(const string &name)
{
    glBindAttribLocation(mProgram, mNumAttributes++, name.c_str());
}

GLint GLProgram::getAttribute(const string &name)
{
    return glGetAttribLocation(mProgram, name.c_str());
}
GLuint GLProgram::getUniformLocation(const std::string &uniformName)
{
    GLint location = glGetUniformLocation(mProgram, uniformName.c_str());
    if ((unsigned)location == GL_INVALID_INDEX)
    {
        cerr << location;
        cerr << "Uniform: " << uniformName << " not found in shader for program " << mProgram << endl;
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
