#version 430
const float FogDensity = 0.75;
const float FogGradient = 3.5;

layout(location=0) in vec3 position;	//aPos
layout(location=1) in vec2 uv;			//aTexCoords
layout(location=2) in vec3 normal;		//aNormal

out vec3 FragPos;						//positionWorld
out vec2 TexCoords;						//uvWorld
out vec3 Normal;						//normalWorld
out float visibility;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 view0;
uniform int FogFlag;

void main()
{
//fog
	if (FogFlag==1) {
		float distance=length(view0 * model * vec4(position, 1.0));
		visibility=exp(-pow(distance*FogDensity, FogGradient));
		visibility=clamp(visibility, 0, 1);
	} else {
		visibility=1;
	}

//main
    FragPos = vec3(model * vec4(position, 1.0));
	//Normal=normal;
    Normal = mat3(transpose(inverse(model))) * normal;  
    TexCoords = uv;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}