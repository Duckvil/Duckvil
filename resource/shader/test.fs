#version 450

out vec4 color;
out unsigned int entityID;

in vec2 texCoord0;
in flat unsigned int entityID0;

uniform sampler2D tex;

void main()
{
    color = texture2D(tex, texCoord0);
    entityID = entityID0;
}