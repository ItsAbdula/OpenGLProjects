#version 420

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 VNormal;
out vec3 VPosition;
out float VNdotv;

uniform vec3 ViewPos;

uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    VPosition = vec3(ModelMatrix * vec4(VertexPosition, 1.0));
    VNormal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;

    vec3 viewDir = normalize(ViewPos - VPosition);
	VNdotv = dot(normalize(VNormal), viewDir);

    VNormal = normalize(NormalMatrix * VertexNormal);
    VPosition = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
