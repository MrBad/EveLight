#version 130
in vec2 position;
in vec4 color;
in vec2 uv;

out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragmentUV;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4 (position, 0.0, 1.0);
    fragmentColor = color;
    fragmentPosition = position;
    fragmentUV = uv;
}
