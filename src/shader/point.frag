#version 460 core

uniform vec3 customColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(customColor, 1.0f);
} 