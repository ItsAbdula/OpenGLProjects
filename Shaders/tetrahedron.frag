#version 420

uniform vec4 LineColor;

in vec3 GPosition;
in vec3 GNormal;

out vec4 FragColor;

void main()
{
    FragColor =  vec4(1.0, 1.0, 1.0, 0.0);

    FragColor = LineColor;
    

}