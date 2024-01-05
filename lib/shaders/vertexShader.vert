#version 430 core  
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 skinIndices;
layout (location = 4) in vec4 skinWeights;

out vec2 textureCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;  
uniform mat4 view;  
uniform mat4 projection;

layout (binding = 0) uniform skinningBuffer
{
    mat4 bones[512];
} skin;

void main()  
{  
    vec4 pos = vec4(aPos, 1.0f);
    vec4 norm = vec4(aNormal, 1.0f);
    vec4 posSkinned = vec4(aPos, 1.0f);  // Initialize to default
    
    // // Check if there are bones influencing the vertex
    // bool hasInfluences = false;
    // for(int i = 0; i < 4; i++)
    // {
    //     if(skinIndices[i] >= 0)
    //     {
    //         hasInfluences = true;
    //         break;
    //     }
    // }

    // if (hasInfluences)
    // {
    //     // Perform skinning calculations
    //     posSkinned = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    //     vec4 normSkinned = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    //     for(int i = 0; i < 4; i++)
    //     {
    //         if(skinIndices[i] >= 0)
    //         {
    //             mat4 bone = skin.bones[skinIndices[i]];
    //             float weight = skinWeights[i];

    //             posSkinned += (bone * pos) * weight;
    //             normSkinned += (bone * norm) * weight;
    //         }
    //     }

    //     posSkinned.w = 1.0f;
    // }

    gl_Position = projection * view * model * posSkinned;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    textureCoord = aTexCoord;
}