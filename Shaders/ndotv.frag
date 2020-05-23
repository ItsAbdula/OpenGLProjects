#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 viewPos;

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	float ndotv = dot(Normal, viewDir);

    FragColor = vec4(ndotv, ndotv, ndotv, 1.0);
} 