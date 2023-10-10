#version 330 core

in vec3 vertColor;
out vec4 color;

vec4 vertexColor = vec4(vertColor, 1.0f);

void main()  
{  
    color = vertexColor;
}