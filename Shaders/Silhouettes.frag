#version 330 core
out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
 
uniform float threshold;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(1.0, 1.0, 1.0);
	if(dot(Normal, viewDir) < threshold)
	{
		result = vec3(0.0, 0.0, 0.0);
	}

    FragColor = vec4(result, 1.0);
} 