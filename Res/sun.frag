#version 450
layout(location = 0)in vec2 UV;
layout(location = 1)in vec3 Norm;
layout(location = 0)out vec4 FragColor;

// layout(location=3)  uniform vec3 DirLight;
struct dirLight{
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
layout(std140,binding = 0) uniform dL{
    dirLight DirLight;
};
void main()
{
	vec4 col = vec4(1,.2 + sin(DirLight.dir.y),.2 + sin(DirLight.dir.y),1.0);
    
    FragColor = col;
}