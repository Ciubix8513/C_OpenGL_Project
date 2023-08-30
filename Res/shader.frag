#version 450
layout(location = 0)in vec2 UV;
layout(location = 1)in vec3 Norm;
layout(location = 2)in vec3 Pos;

layout(location = 0)out vec4 FragColor;

layout(location = 3)uniform vec3 camPos;

struct dirLight{
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


layout(location = 4) uniform int LightNum;
layout(std140,binding = 0) uniform dL{
    dirLight DirLight;
};
layout(std430, binding = 0) buffer Lights
{
    PointLight lights[];
};

vec3 DirLightF(dirLight dir, vec3 norm, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(dir.dir);

    float dif = max(dot(norm,lightDir),0.0);
    
    vec3 reflDir= reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflDir),0.0), 10.0);

    vec3 amb = dir.ambient * color;
    vec3 diffuse = dir.diffuse * dif * color;
    vec3 specular = dir.specular * spec * color;

    return amb + diffuse + specular;
}

vec3 pointLight(PointLight light,vec3 norm, vec3 pos, vec3 viewDir,vec3 color)
{
    vec3 lightDir = normalize(light.position - pos);

    float dif = max(dot(norm,lightDir),0.0);
    vec3 reflDir = reflect(-lightDir,norm);   
    float spec = pow(max(dot(viewDir,reflDir),0.0),10.0);

    float dist = length(light.position - pos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 amb = light.ambient * color * attenuation;
    vec3 diffuse = light.diffuse * dif * color* attenuation;
    vec3 specular = light.specular * spec * color* attenuation;

    return amb * diffuse * specular;
}

void main()
{
	vec3 col = vec3(1);
    vec3 vDir = normalize(camPos - Pos);

	vec3 res = DirLightF(DirLight,Norm,vDir, col);
    for(int i = 0; i < LightNum; i++)
    res += pointLight(lights[i],Norm,Pos,vDir,col);
        
    
    FragColor = vec4(res,1.0);
}