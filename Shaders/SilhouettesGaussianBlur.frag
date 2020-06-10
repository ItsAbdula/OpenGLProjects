#version 420 core
in vec3 FragPos;  
in vec3 Normal;  

uniform int Pass;

layout(binding = 0) uniform sampler2D Texture0;

uniform float threshold;
uniform vec3 viewPos;

layout(location = 0) out vec4 FragColor;

uniform float Weight[5];

vec3 silhouettes(vec3 pos, vec3 norm)
{
    vec3 viewDir = normalize(viewPos - pos);

	vec3 result = vec3(1.0, 1.0, 1.0);
	if(dot(normalize(norm), viewDir) < threshold)
	{
		result = vec3(0.0, 0.0, 0.0);
	}

    return result;
}

vec4 pass1()
{
    return vec4(silhouettes(FragPos, normalize(Normal)), 1.0);
}

vec4 pass2()
{
    ivec2 pix = ivec2(gl_FragCoord.xy);
    vec4 sum = texelFetch(Texture0, pix, 0) * Weight[0];

	for(int i = 1; i <= 4; i++)
	{
		sum += texelFetchOffset(Texture0, pix, 0, ivec2(0,i)) * Weight[i];
		sum += texelFetchOffset(Texture0, pix, 0, ivec2(0,-i)) * Weight[i];
	}

    return sum;
}

vec4 pass3()
{
    ivec2 pix = ivec2(gl_FragCoord.xy);
    vec4 sum = texelFetch(Texture0, pix, 0) * Weight[0];

	for(int i = 1; i <= 4; i++)
	{
	    sum += texelFetchOffset(Texture0, pix, 0, ivec2(i,0)) * Weight[i];
		sum += texelFetchOffset(Texture0, pix, 0, ivec2(-i,0)) * Weight[i];
	}

    return sum;
}

void main()
{
    if(Pass == 1) FragColor = pass1();
    else if(Pass == 2) FragColor = pass2();
    else if(Pass == 3) FragColor = pass3();
} 