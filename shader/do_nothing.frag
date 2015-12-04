#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    // Ambient
    vec3 result = vec3(0.5,0.5,0.5);
    color = vec4(result, 1.0f);
}