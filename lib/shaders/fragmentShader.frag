#version 330 core

//in vec3 vertColor;
in vec2 textureCoord;
out vec4 color;

uniform sampler2D ourTexture;
uniform vec4 customColor;

void main()  
{  
    color = texture(ourTexture, textureCoord) * customColor;
}