#version 430
out vec4 FragColor;

struct Material {
    sampler2D emission_0;
    sampler2D diffuse_0;

	sampler2D emission_1;
    sampler2D diffuse_1;

	sampler2D specular_0;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;					//positionWorld
in vec2 TexCoords;					//uvWorld
in vec3 Normal;						//normalWorld
in float visibility;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec4 FogRealColor;

void main()
{
    // emission
    vec3 emission = (0.5*texture(material.emission_0, TexCoords)+0.5*texture(material.emission_1, TexCoords)).rgb;

    // ambient
    vec3 ambient = light.ambient * (0.5*texture(material.diffuse_0, TexCoords)+0.5*texture(material.diffuse_1, TexCoords)).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * (0.5*texture(material.diffuse_0, TexCoords)+0.5*texture(material.diffuse_1, TexCoords)).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular_0, TexCoords).rgb;
        
    vec3 result = emission + ambient + diffuse + specular;
    vec4 daColor = vec4(result, 1.0);
	FragColor = mix(FogRealColor, daColor, visibility);
} 