#version 130

varying vec2 texCoord0;

uniform sampler2D tex;

void main()
{
    gl_FragColor = texture2D(tex, texCoord0);
}