#version 450 core

out vec4 color;

in vec4 fragmentColor;

void main()
{
    color = fragmentColor;    
} 