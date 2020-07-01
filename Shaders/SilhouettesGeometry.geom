#version 420

// layout(INPUT_PRIMITIVE) in;
layout(triangles) in;

// layout(OUTPUT_PRIMITIVE​, max_vertices = VERTEX_COUNT​) out;
layout(triangle_strip, max_vertices = 15) out;

out vec3 GPosition;
out vec3 GNormal;

flat out int GIsEdge; // all int need to be flat

in float VNdotv[];

uniform float EdgeWidth;
uniform float PctExtend;

float inverseLerp(float a, float b)
{
    return -b / (a - b);
}

vec3 interpolation(vec3 begin, vec3 end, float ndotvBegin, float ndotvEnd)
{
    return mix(begin, end, inverseLerp(ndotvBegin, ndotvEnd));
}

void emitEdgeQuad(vec3 e0, vec3 e1)
{
    vec2 ext = PctExtend * (e1.xy - e0.xy);
    vec2 v = normalize(e1.xy - e0.xy);
    vec2 n = vec2(-v.y, v.x) * EdgeWidth;

    GIsEdge = 1;

    gl_Position = vec4(e0.xy - ext - n, e0.z, 1.0);
    EmitVertex();

    gl_Position = vec4(e0.xy - ext + n, e0.z, 1.0);
    EmitVertex();

    gl_Position = vec4(e1.xy + ext - n, e1.z, 1.0);
    EmitVertex();

    gl_Position = vec4(e1.xy + ext + n, e1.z, 1.0);
    EmitVertex();

    EndPrimitive();
}

void drawSilhouettes(vec3 vertices[3])
{
    vec3 pos1;
    vec3 pos2;

    if(VNdotv[0] * VNdotv[1] < 0 && VNdotv[0] * VNdotv[2] < 0)
    {
        pos1 = interpolation(vertices[0], vertices[1], VNdotv[0], VNdotv[1]);
        pos2 = interpolation(vertices[0], vertices[2], VNdotv[0], VNdotv[2]);

        emitEdgeQuad(pos1, pos2);
    }
    else if(VNdotv[1] * VNdotv[0] < 0 && VNdotv[1] * VNdotv[2] < 0)
    {
        pos1 = interpolation(vertices[1], vertices[0], VNdotv[1], VNdotv[0]);
        pos2 = interpolation(vertices[1], vertices[2], VNdotv[1], VNdotv[2]);

        emitEdgeQuad(pos1, pos2);
    }
    else if(VNdotv[2] * VNdotv[0] < 0 && VNdotv[2] * VNdotv[1] < 0)
    {
        pos1 = interpolation(vertices[2], vertices[0], VNdotv[2], VNdotv[0]);
        pos2 = interpolation(vertices[2], vertices[1], VNdotv[2], VNdotv[1]);

        emitEdgeQuad(pos1, pos2);
    }
}


void main()
{
    vec3 vertices[3];
    for(int i = 0; i < 3; i++)
    {
        vertices[i] = gl_in[i].gl_Position.xyz / gl_in[i].gl_Position.w;
    }
    drawSilhouettes(vertices);

    GIsEdge = 0;

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
