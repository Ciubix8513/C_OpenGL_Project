#version 450
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iUV;
layout(location =2) in vec3 iNorm;

layout(location = 0)out vec2 oUV;
layout(location = 1)out vec3 oNorm;


layout(location = 0) uniform mat4 Projection;
layout(location = 1)uniform mat4 Camera;
layout(location = 2) uniform mat4 World;
// layout(location = 3) uniform float iTime;
mat4 rotMat(vec3 r)
{
    mat4 roll = mat4(
    vec4(1,0,0,0),
    vec4(0,cos(r.x),-sin(r.x),0),
    vec4(0,sin(r.x),cos(r.x),0),
    vec4(0,0,0,1));
    mat4 pitch = mat4(
    vec4(cos(r.y),0,sin(r.y) ,0),
    vec4(0,1,0,0),
    vec4(-sin(r.y),0,cos(r.y),0),
    vec4(0,0,0,1));
    mat4 yaw = mat4(
    vec4(cos(r.z),-sin(r.z),0,0),
    vec4(sin(r.z),cos(r.z),0,0),
    vec4(0,0,1,0),
    vec4(0,0,0,1));   
    
    return roll * pitch * yaw;    
}

void main()
{	
	vec4 pos = vec4(iPos,1.0);
    pos = World * pos;
	pos = Camera * pos;
    
	pos = Projection * pos;
	gl_Position = pos;
	
	oUV = iUV;
	oNorm = iNorm;
}