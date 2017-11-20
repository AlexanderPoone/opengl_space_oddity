#version 430
const float FogDensity = 0.75;
const float FogGradient = 3.5;
layout (location = 0) in vec3 aPos;

out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int FogFlag;

void main()
{
//fog
	if (FogFlag==1) {
		float distance=length(view * model * vec4(aPos, 1.0));
		visibility=exp(-pow(distance*FogDensity, FogGradient));
		visibility=clamp(visibility, 0, 1);
	} else {
		visibility=1;
	}

//main
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}