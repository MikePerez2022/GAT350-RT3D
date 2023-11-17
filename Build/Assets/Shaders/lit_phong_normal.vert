#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;
in layout(location = 3) vec3 vtangent;
//layout(location = 4) in vec3 inBitangent;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec2 otexcoord;
out layout(location = 2) vec4 oshadowcoord;//Take in shadow coords
out layout(location = 3) mat3 otbn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 shadowVP;//The Shadow view projection mat 4

uniform struct Material
{
	uint params;
	vec3 albedo;
	vec3 specular;
	vec3 emissive;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material;


void main()
{
	mat4 modelView = view * model;

	//Convert position and normal to world-view space
	oposition = vec3(modelView * vec4(vposition, 1));

	//Calculate tbn matrix
	vec3 normal = normalize(mat3(modelView) * vnormal);	
	vec3 tangent = normalize(mat3(modelView) * vtangent);
	vec3 bitangent = cross(normal, tangent);
	
	otbn = mat3(tangent, bitangent, normal);

	otexcoord = (vtexcoord * material.tiling) + material.offset;
	oshadowcoord = shadowVP * model * vec4(vposition, 1);//Transform the cood positions from model to shadow view projection


	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
