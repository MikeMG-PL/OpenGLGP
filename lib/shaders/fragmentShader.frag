#version 330 core

in vec2 textureCoord;
out vec4 color;

uniform sampler2D ourTexture;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform vec4 customColor;

void main()  
{  
    color = texture(ourTexture, textureCoord) * customColor;
}