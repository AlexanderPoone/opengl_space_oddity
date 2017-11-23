#version 430
const float FogDensity = 0.75;
const float FogGradient = 3.5;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;		//aNormal
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;
out float visibility;

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
    TexCoords = uv;
    gl_Position = projection * view * aInstanceMatrix * vec4(position, 1.0f); 
}