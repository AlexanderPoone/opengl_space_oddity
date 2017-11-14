#version 430

layout(location=0) in vec3 position;	//aPos
layout(location=1) in vec2 uv;			//aTexCoords
layout(location=2) in vec3 normal;		//aNormal

out vec3 FragPos;						//positionWorld
out vec2 TexCoords;						//uvWorld
out vec3 Normal;						//normalWorld

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main()
{
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;  
    TexCoords = uv;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}