#version 420

layout(triangles) in;

layout(triangle_strip, max_vertices = 8) out;

uniform float height;

void emitTetrahedron(vec3 vertices[4])
{
    gl_Position = vec4(vertices[0], 1.0);
    EmitVertex();

    gl_Position = vec4(vertices[1], 1.0);
    EmitVertex();

    gl_Position = vec4(vertices[2], 1.0);
    EmitVertex();

    gl_Position = vec4(vertices[3], 1.0);
    EmitVertex();
	
	gl_Position = vec4(vertices[0], 1.0);
    EmitVertex();

    EndPrimitive();
}


void main()
{
    vec3 vertices[3];
    for(int i = 0; i < 3; i++)
    {
        vertices[i] = gl_in[i].gl_Position.xyz / gl_in[i].gl_Position.w;
    }
    
    vec3 tetrahedron[4];
    tetrahedron[0] = (vertices[0] + vertices[1]) / 2.0;
    tetrahedron[1] = (vertices[0] + vertices[2]) / 2.0;
    tetrahedron[2] = (vertices[1] + vertices[2]) / 2.0;

	tetrahedron[3] = (vertices[0] + vertices[1] + vertices[2]) / 3.0;
	tetrahedron[3] += cross(vertices[0] - vertices[1], vertices[2] - vertices[1]) * height;
	
	emitTetrahedron(tetrahedron);

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}