#version 430
const float FogDensity = 0.75;
const float FogGradient = 3.5;
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
out float visibility;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 view0;
uniform int FogFlag;

void main()
{
//fog
	if (FogFlag==1) {
		float distance=length(view0 * vec4(aPos, 1.0));
		visibility=exp(-pow(distance*FogDensity, FogGradient));
		visibility=clamp(visibility, 0, 1);
	} else {
		visibility=1;
	}

//main
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  