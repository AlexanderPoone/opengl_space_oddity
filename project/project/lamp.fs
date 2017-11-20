#version 430
out vec4 FragColor;
uniform vec4 FogRealColor;

in float visibility;

void main()
{
    vec4 daColor = vec4(1.0); // set alle 4 vector values to 1.0
	FragColor = mix(FogRealColor, daColor, visibility);
}