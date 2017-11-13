#version 430
// FragmentShader.glsl Diffuse+Specular

out vec4 daColor;
in vec3 vertexPositionWorld;
in vec2 uvWorld;
in vec3 normalWorld;

uniform vec3 ambientLight;
uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;

uniform vec3 diffuseIncrement;
uniform float specularExponent;	//ns

uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSampler2;
//uniform sampler2DShadow shadowMap;

void main() {
	//Diffuse
	vec3 lightVectorWorld=normalize(lightPositionWorld-vertexPositionWorld);
	float brightness=dot(lightVectorWorld, normalize(normalWorld));
	vec3 diffuseLight=vec3(brightness, brightness, brightness);

	//Specular
	//Calculate reflect light direction
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	//Calculate direction from eye to object
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	//Calculate light brightness according to the angle between eye and reflect light
	float s=clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	//Control lobe of specularLight
	s=pow(s, specularExponent);
	vec3 specularLight=vec3(s,s,s);

	vec3 materialAmbientColor = 0.5 * texture( myTextureSampler, uvWorld ).rgb + 0.5 * texture( myTextureSampler2, uvWorld ).rgb;
	vec3 materialDiffuseColor = 0.5 * texture( myTextureSampler, uvWorld ).rgb + 0.5 * texture( myTextureSampler2, uvWorld ).rgb;
	vec3 materialSpecularColor = vec3(.4,.4,.4);

	//float visibility = texture( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w) );

	//vec3 color=ambientLight+clamp(diffuseLight,0,1)+specularLight;
	vec3 color=materialAmbientColor*ambientLight + materialDiffuseColor*clamp(diffuseLight,0,1)*diffuseIncrement + materialSpecularColor*specularLight;

	daColor=vec4(color, 1.0);
	//daColor=visibility*vec4(color, 1.0);
}