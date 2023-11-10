#version 430

#define INVERT_MASK		 (1 << 0)
#define GREYSCALE_MASK	 (1 << 1)
#define COLORTINT_MASK	 (1 << 2)
#define GRAIN_MASK		 (1 << 3)
#define SCANLINE_MASK	 (1 << 4)
#define EDGE_DETECTION	 (1 << 5)

in layout(location = 0) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform vec3 tint;
uniform uint params = 0;

layout(binding = 0) uniform sampler2D screenTexture;

float rand(vec2 st)
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 invert(vec4 color)
{
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 greyscale(vec4 color)
{
	return vec4(vec3((((0.2126 * color.r) + (0.7152 * color.g) + (0.0722 * color.b))/ 3)), color.a);
}

vec4 colortint(vec4 color)
{
	return color * vec4(tint, 1);
}

vec4 grain(vec4 color)
{
    float noise = (rand(ftexcoord) * 2.0 - 1.0) * 0.5;// 0.5 = grain amount
    return color + noise;
}

vec4 scanline(vec4 color)
{
	int scanline = int(ftexcoord.y * 460) % 2;// 460 = frequency
	vec3 scanlineColor = vec3(0.0, 0.0, 0.0);
	return mix(color, vec4(scanlineColor, color.a), float(scanline));
}

vec4 edge(vec4 color)
{
	//Kernel
	mat3 edge = mat3(
    1.0, 1.0, 1.0,
    1.0, -8.0,1.0,
    1.0, 1.0, 1.0  
	);

    vec3 outline = color.rgb * edge[1][1]; //multiply it by the center value

	//I used some outside resources and code to get the calulations for the nested for loop.
	//I didn't know engough to do it without research, but I did mod the calulations so it's understadible.

	//iterates over the rows
    for (int i = -1; i <= 1; ++i) 
	{
		//iterates over the columns
		for (int j = -1; j <= 1; ++j) 
		{
			if (i != 0 || j != 0) 
			{
				vec2 texturesize = textureSize(screenTexture, 0);
				vec2 edgeOffsets = vec2(float(i), float(j));
				vec3 image = texture(screenTexture, ftexcoord + edgeOffsets / texturesize).rgb;
				outline += image * edge[i+1][j+1];//you add 1 because of the for loops 
			}
		}
    }

	return vec4(outline, 1.0);
}

void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & INVERT_MASK)) postprocess = invert(postprocess);
	if (bool(params & GREYSCALE_MASK)) postprocess = greyscale(postprocess);
	if (bool(params & COLORTINT_MASK)) postprocess = colortint(postprocess);
	if (bool(params & GRAIN_MASK)) postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK)) postprocess = scanline(postprocess);
	if (bool(params & EDGE_DETECTION)) postprocess = edge(postprocess);

	ocolor = mix(basecolor, postprocess, blend);
}
