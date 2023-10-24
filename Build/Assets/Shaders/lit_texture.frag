#version 430

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec2 ftexcoord;
in layout(location = 2) vec3 fnormal;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

uniform float time;

//Material
uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material;

//Light
uniform struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float cutoff;
} light;

uniform vec3 ambientLight;

vec3 ads( vec3 position, vec3 normal)
{
	//AMBIENT
	vec3 ambient = ambientLight;


	//DIFFUSE					  //DIRECTIONAL == 1
	vec3 lightDir = (light.type == 1) ? normalize(-light.direction) : normalize(light.position - position);

	float spotIntensity = 1.0;
	if(light.type == 2) //SPOT == 2
	{
		float angle = acos(dot(light.direction, -lightDir));
		if (angle > light.cutoff) spotIntensity = 0;
	}

	float intensity = max(dot(lightDir, normal), 0) * spotIntensity;
	vec3 diffuse = material.diffuse * (light.color * intensity);


	//SPECULAR
	vec3 specular = vec3(0);
	if(intensity > 0)
	{
		vec3 reflection = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-position);
		intensity = max(dot(reflection, viewDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;
	}

	return ambient + diffuse + specular;
};

void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
	
}
