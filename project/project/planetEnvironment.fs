#version 430
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in float visibility;

uniform vec3 viewPos;
uniform samplerCube skybox;
uniform vec4 FogRealColor;

void main()
{    
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 daColor = vec4(texture(skybox, R).rgb, 1.0);
	FragColor = mix(FogRealColor, daColor, visibility);
}