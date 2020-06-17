#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out float NdotV;

uniform vec3 ViewPos;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    FragPos = vec3(ModelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(ModelMatrix))) * aNormal;

    vec3 viewDir = normalize(ViewPos - FragPos);
	NdotV = dot(normalize(Normal), viewDir);

    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}