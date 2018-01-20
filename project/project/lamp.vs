#version 430
const float FogDensity = 1.25;
const float FogGradient = 0.75;
layout (location = 0) in vec3 aPos;

out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 view0;
uniform mat4 projection;
uniform int FogFlag;

void main()
{
//fog
	if (FogFlag==1) {
		float distance=length(view0 * model * vec4(aPos, 1.0));
		visibility=exp(-pow(distance*FogDensity, FogGradient));
		visibility=clamp(visibility, 0, 1);
	} else {
		visibility=1;
	}

//main
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}