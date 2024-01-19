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

layout(local_size_x = 100, local_size_y = 1, local_size_z = 1) in;

uniform float dt;
uniform float accelerationStrenght;

uniform vec3 minSpeedColor;
uniform vec3 maxSpeedColor;
uniform float highSpeedMax;

uniform vec3 pos1;
uniform vec3 pos2;

uniform vec3 implosionColor;
uniform float implosionColorDistMax;

uniform int sphereActivate = 1;
uniform float sphereRadius = 50.0;
uniform float sphereBounce = 0.2; 

vec3 accelerate_towards(vec3 pos, vec3 target, out float dist)
{
    vec3 dir = target - pos;
    dist = length(dir);
    return accelerationStrenght * dir / (dist * dist);
}

void main()
{
    uint gid = gl_GlobalInvocationID.x;

    vec3 position = Positions[gid].xyz;
    vec3 velocity = Velocities[gid].xyz;

    // Calculate accelerations towards blackholes
    float distToPos1, distToPos2;
    vec3 acceleration = accelerate_towards(position, pos1, distToPos1);
    acceleration += accelerate_towards(position, pos2, distToPos2);

    // Calculte new position & velocity
    vec3 new_position = position + velocity * dt + 0.5 * dt * dt * acceleration;
    vec3 new_velocity = velocity + acceleration * dt;
    float new_velocity_length = length(new_velocity);

    // Apply in-sphere effect
    if (sphereActivate == 1 && length(new_position) > sphereRadius)
    {
        new_position = normalize(new_position) * sphereRadius;
        new_velocity = new_velocity_length * normalize(-new_position) * sphereBounce;
    }

    // Apply new position & velocity
    Positions[gid].xyz = new_position;
    Velocities[gid].xyz = new_velocity;

    // Find Color
    vec3 color = vec3(1.0);
    color = mix( minSpeedColor, maxSpeedColor, min(new_velocity_length / highSpeedMax, 1.0));  //  colorize depending on velocity
    color = mix( color, implosionColor, 1.0 - min(min(distToPos1 / implosionColorDistMax, 1.0) * min(distToPos2 / implosionColorDistMax, 1.0), 1.0));  //  color if close or not
    Colors[gid].xyz = color;
    
}