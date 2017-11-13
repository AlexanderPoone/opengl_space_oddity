#version 430
// VertexShader.glsl Ambient+Diffuse

in layout(location=0) vec3 position;
in layout(location=1) vec2 uv;
in layout(location=2) vec3 normal;

uniform vec3 ambientLight;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 positionWorld;
out vec2 uvWorld;
out vec3 normalWorld;
//out vec4 ShadowCoord;

void main() {
	vec4 v=vec4(position, 1.0);
	vec4 newPosition = model * v;
	vec4 projectedPosition=view * projection * newPosition;
	gl_Position=projectedPosition;

	//ShadowCoord = DepthBiasMVP * projectedPosition;

	vec4 normal_temp=modelTransformMatrix*vec4(normal,0);
	normalWorld=normal_temp.xyz;

	positionWorld=newPosition.xyz;
	uvWorld=uv;
}