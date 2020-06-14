#version 420

// layout(INPUT_PRIMITIVE) in;
layout(triangles_adjacency) in;

// layout(OUTPUT_PRIMITIVE​, max_vertices = VERTEX_COUNT​) out;
layout(triangle_strip, max_vertices = 15) out;

out vec3 GPosition;
out vec3 GNormal;

// Which triangle edges are silhouette edges
flat out int GIsEdge; // all int need to be flat

in vec3 VNormal[];
in vec3 VPosition[];

uniform float EdgeWidth;
uniform float PctExtend;

bool isFrontFacing(vec3 a, vec3 b, vec3 c)
{
    return ((a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y))
            > 0;
}

void emitEdgeQuad(vec3 e0, vec3 e1)
{
    vec2 ext = PctExtend * (e1.xy - e0.xy);
    vec2 v = normalize(e1.xy - e0.xy);
    vec2 n = vec2(-v.y, v.x) * EdgeWidth;

    GIsEdge = 1;   // This is part of the sil. edge

    gl_Position = vec4(e0.xy - ext, e0.z, 1.0);
    EmitVertex();

    gl_Position = vec4(e0.xy - n - ext, e0.z, 1.0);
    EmitVertex();

    gl_Position = vec4(e1.xy + ext, e1.z, 1.0);
    EmitVertex();

    gl_Position = vec4(e1.xy - n + ext, e1.z, 1.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    vec3 vertices[6];
    for(int i = 0; i < 6; i++)
    {
        vertices[i] = gl_in[i].gl_Position.xyz / gl_in[i].gl_Position.w;
    }

    if(isFrontFacing(vertices[0], vertices[2], vertices[4]))
    {
        if(isFrontFacing(vertices[0],vertices[1],vertices[2]) == false)
            emitEdgeQuad(vertices[0], vertices[2]);

        if(isFrontFacing(vertices[2],vertices[3],vertices[4]) == false)
            emitEdgeQuad(vertices[2], vertices[4]);

        if(isFrontFacing(vertices[4],vertices[5],vertices[0]) == false)
            emitEdgeQuad(vertices[4], vertices[0]);
    }

    // Output the original triangle

    GIsEdge = 0;   // This triangle is not part of an edge.

    GNormal = VNormal[0];
    GPosition = VPosition[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    GNormal = VNormal[2];
    GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    GNormal = VNormal[4];
    GPosition = VPosition[4];
    gl_Position = gl_in[4].gl_Position;
    EmitVertex();

    EndPrimitive();
}
