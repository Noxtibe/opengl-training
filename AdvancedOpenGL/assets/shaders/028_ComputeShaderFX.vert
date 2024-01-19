#version 450 core

layout(std430, binding = 3) buffer layout_Positions
{
    vec4 Positions[];
};
layout(std430, binding = 4) buffer layout_Velocities
{
    vec4 Velocities[];
};
layout(std430, binding = 5) buffer layout_Colors
{
    vec4 Colors[];
};

uniform mat4 view;
uniform mat4 projection;
uniform float pointSize;

out vec4 fragmentColor;

void main()
{
    gl_Position = projection * view * Positions[gl_VertexID];
    gl_PointSize = pointSize / gl_Position.z;

    fragmentColor = Colors[gl_VertexID];
}