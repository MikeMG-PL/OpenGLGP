#version 440 core

in vec2 textureCoord;
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;

uniform vec4 customColor;

void main()  
{  
    color = texture(texture_diffuse1, textureCoord) * customColor;
}