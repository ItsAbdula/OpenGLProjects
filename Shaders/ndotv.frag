#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;
in float NdotV;

void main()
{
    FragColor = vec4(NdotV, NdotV, NdotV, 1.0);
} 