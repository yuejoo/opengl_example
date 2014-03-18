#version 150 

in  vec4 vPosition;
out vec4 color;

uniform vec4 vColor;
uniform mat4 modelview;
uniform mat4 projection;

void main() 
{
    color = vColor;
    gl_Position = projection*modelview*vPosition;
} 
