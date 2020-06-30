#version 420

uniform int onlySilhouette;
uniform vec4 LineColor;

in vec3 GPosition;
in vec3 GNormal;

flat in int GIsEdge; // all int need to be flat

out vec4 FragColor;

void main()
{
    FragColor =  vec4(1.0, 1.0, 1.0, 0.0);
    if(GIsEdge == 1)
    {
        FragColor = LineColor;
    }
   
    if(onlySilhouette == 1)
    {
        if(GIsEdge == 0) discard;

        FragColor = LineColor;
    }
}