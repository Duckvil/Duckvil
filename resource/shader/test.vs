#version 450

in vec3 position;
in vec2 texCoord;
in unsigned int entityID;

out vec2 texCoord0;
out flat unsigned int entityID0;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    texCoord0 = texCoord;
    entityID0 = entityID;
}