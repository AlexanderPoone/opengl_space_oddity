#version 430
out vec4 FragColor;

in vec2 TexCoords;
in float visibility;

uniform sampler2D texture_diffuse1;
uniform vec4 FogRealColor;

void main()
{
    vec4 daColor = texture(texture_diffuse1, TexCoords);
	FragColor = mix(FogRealColor, daColor, visibility);
}