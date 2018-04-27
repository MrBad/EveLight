/**
 * A small test game
 */
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "entity.h"
#include "balls.h"

bool Balls::onGameInit()
{
    const char *vertex_shader =
        "#version 130\n"
        "in vec2 position;"
        "void main() {"
        "  gl_Position = vec4(position, 0.0, 1.0);"
        "}";

    const char *fragment_shader =
        "#version 130\n"
        "out vec4 out_color;"
        "void main() {"
        "  out_color = vec4(0.5, 0.0, 0.2, 1.0);"
        "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    GLuint mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vs);
    glAttachShader(mShaderProgram, fs);
    glLinkProgram(mShaderProgram);
    glDetachShader(mShaderProgram, vs);
    glDetachShader(mShaderProgram, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(mShaderProgram);

    float points[] = {
        0.0f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f,
    };

    mVertexArray = 0;
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    mVertexBuffer = 0;
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLint posAttr = glGetAttribLocation(mShaderProgram, "position");
    glEnableVertexAttribArray(posAttr);
    glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    return true;
}

bool Balls::onGameUpdate(uint32_t ticks)
{
    (void)ticks;

    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(mShaderProgram);
    glBindVertexArray(mVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //std::cout << mFPS << std::endl;
    return true;
}

void Balls::onGameDelete()
{
}
