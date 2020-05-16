#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 ourColor;
//
//void main()
//{
//   gl_Position = vec4(aPos, 1.0);
//   ourColor = aColor;
//}

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColor;

out vec3 Color;

uniform float Ratio;

void main()
{
	Color = VertexColor;
	
	vec3 newVertexPosition = VertexPosition;
    newVertexPosition.z *= newVertexPosition.z * (1.0 - Ratio);

	gl_Position = vec4(newVertexPosition, 1.0);
}
