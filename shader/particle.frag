#version 330 core
out vec4 color;

uniform vec4 color_calculate;

void main()
{
    color = color_calculate;
}