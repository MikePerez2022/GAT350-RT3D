#version 430

in layout(location = 0) vec3 vposition;
out layout(location = 0) vec3 otexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 vreflect(vec3 i, vec3 n)
{
	return i - (n * dot(n,i)) * 2;
}


void main()
{
	//transfom vertex position/normal in worls space
	vec3 position = vec3(model * vec4(vposition,1));
	vec3 normal = normalize(mat3(model) * vposition);

	//Inverse of view space >> world space
	//Last column of main is position
	vec3 viewPosition = inverse(view)[3].xyz;
	vec3 viewDir = normalize(position - viewPosition);

	//Reflect view direction about vertex normal
	otexcoord = vreflect(viewDir, normal);

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
