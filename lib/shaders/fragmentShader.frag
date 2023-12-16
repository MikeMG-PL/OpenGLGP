#version 440 core

in vec2 textureCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;

uniform int wireframe;
uniform vec4 customColor;
uniform vec3 viewPos;

struct PointLight {    
        vec3 position;

        float constant;
        float linear;
        float quadratic;  

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };  


struct DirLight {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

uniform DirLight dirLight;

uniform int numPointLights;
uniform PointLight pointLights[512]; // Allow max 512 lights

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()  
{  
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    if(numPointLights > 0)
    {
        for(int i = 0; i < numPointLights; i++)
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

    if(wireframe == 0)
        color = vec4(result, 1.0);
    else
        color = customColor;

    //color = texture(texture_diffuse1, textureCoord) * customColor;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    // No materials yet, so SHININESS is hardcoded

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f); // Last param is shininess

    // po³¹cz wyniki
    vec3 ambient = light.ambient  * vec3(texture(texture_diffuse1, textureCoord));
    vec3 diffuse = light.diffuse  * diff * vec3(texture(texture_diffuse1, textureCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, textureCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // œwiat³o rozproszone
    float diff = max(dot(normal, lightDir), 0.0);
    // œwiat³o lustrzane
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // t³umienie
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
      			        light.quadratic * (distance * distance));    
    // po³¹cz wyniki
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, textureCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, textureCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, textureCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}