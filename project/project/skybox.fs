#version 430
out vec4 FragColor;

in vec3 TexCoords;
in float visibility;

uniform samplerCube skybox;
uniform vec4 FogRealColor;

void main()
{    
    vec4 daColor = texture(skybox, TexCoords);
	FragColor = mix(FogRealColor, daColor, visibility);
}