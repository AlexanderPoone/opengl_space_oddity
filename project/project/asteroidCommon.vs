#version 430
const float FogDensity = 1.25;
const float FogGradient = 0.75;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;		//aNormal
layout (location = 3) in mat4 aInstanceMatrix;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out float visibility;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 view0;
uniform int FogFlag;

void main()
{
//fog
	if (FogFlag==1) {
		float distance=length(view0 * vec4(position, 1.0));
		visibility=exp(-pow(distance*FogDensity, FogGradient));
		visibility=clamp(visibility, 0, 1);
	} else {
		visibility=1;
	}

//main
	//new
	FragPos = vec3(transform * aInstanceMatrix * vec4(position, 1.0f));
	//FragPos = vec3(aInstanceMatrix * vec4(position, 1.0f));
    TexCoords = uv;
	//new
	Normal = mat3(transpose(inverse(aInstanceMatrix))) * normal;  
    gl_Position = projection * view * vec4(FragPos, 1.0); 
}