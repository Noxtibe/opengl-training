#version 450 core

layout(location = 0) in vec3 inPosition;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    gl_PointSize = 4.0; // Ajustez la taille du point selon vos préférences
}
