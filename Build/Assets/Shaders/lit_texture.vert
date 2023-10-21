#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec2 otexcoord;
out layout(location = 2) vec3 onormal;
out layout(location = 3) vec4 ocolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	mat4 modelView = view * model;

	//Convert position and normal to world-view space then pass on to frag shader
	oposition = vec3(modelView * vec4(vposition, 1));
	onormal = normalize(mat3(modelView) * vnormal);

	//adjust texture cordinates to the offset and tiling
	otexcoord = vtexcoord;
	
	//pass the matrial color down the pipeline
	ocolor = vec4(1,1,1,1);

	//place model into world-view space
	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
