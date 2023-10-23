#version 430

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec2 ftexcoord;
in layout(location = 2) vec3 fnormal;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

uniform float time;

//Material - some user modifiable attributes
uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material;

//Light -- attributes
uniform struct Light
{
	vec3 position;
	vec3 color;
} light;

uniform vec3 ambientLight;

vec3 ads( vec3 position, vec3 normal)
{
	//AMBIENT - background lighting
	vec3 ambient = ambientLight;


	//DIFFUSE
	//Find the direction the light is pointing by normalizing it
	vec3 lightDir = normalize(light.position - position);
	//Find the intensity by checking the light direction against the surface normal and clapming it between 1 and 0
	float intensity = max(dot(lightDir, normal), 0);
	//Multiply the material color by the light color and intensity to get the diffuse
	vec3 diffuse = material.diffuse * (light.color * intensity);


	//SPECULAR
	vec3 specular = vec3(0);
	if(intensity > 0)
	{
		//Calculates how the light reflects based on the light direction and the surface normal
		vec3 reflection = reflect(-lightDir, normal);
		//Normalizes the direction the camerra view has
		vec3 viewDir = normalize(-position);
		//Calculates the intesity based on the reflection and the cameras view direction
		intensity = max(dot(reflection, viewDir), 0);
		//Modifies the intensity by the shininess of the model's material
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;
	}
	//Adding all the lighting to it
	return ambient + diffuse + specular;
};

void main()
{
	//Makes texcord into texture and allows it to adjust to tiling and offset
	vec4 texcolor = texture(tex, ftexcoord);
	//Apply lighting calculations to the color of the object
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
	
}
