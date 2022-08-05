#version 450
layout(location = 0)in vec2 UV;
layout(location = 1)in vec3 Norm;
layout(location = 0)out vec4 FragColor;
layout(location = 4) uniform vec2 iRes;

uniform sampler2D tex;

vec4 getCol(float coord,int ColNum)
{ 
   
   vec4[] cols =vec4[] (vec4(85,205,252,255),vec4(247,168,184,255),vec4(255),vec4(247,168,184,255),vec4(85,205,252,255));       

    int arrLength = 5;
    
    if(ColNum == 1) 
        return cols[0];
        
    float cstep1 = 1.0 / float(ColNum - 1);//Num of subgradients = num of colors - 1
    
    for(int i = 1; i < ColNum; i++)
    {
        if(coord < cstep1 * float(i))
        return mix(cols[int(mod(float(i-1),float(arrLength)))],cols[int(mod(float(i),float(arrLength)))], coord / cstep1 - float (i - 1));
    }    
    return vec4(coord);
}

void main()
{
	vec4 col = texture2D(tex,UV); 
    vec3 sun = vec3(0,1,-1);
    col *= max(.2, dot(Norm,sun));
  
    
    FragColor = col;
}