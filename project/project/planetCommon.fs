#version 430
out vec4 FragColor;

struct Material {
    sampler2D emission;
    sampler2D diffuse;
    sampler2D specular;    
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
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // emission
    vec3 emission = texture(material.emission, TexCoords).rgb;

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        
    vec3 result = emission + ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 