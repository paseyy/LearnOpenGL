#version 330 core
in vec3 vNormal;
in vec3 vPos;
in vec2 vTexCoords;

out vec4 fColor;


struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
	float emissionStrength;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


uniform vec3 viewPos;

uniform Material cubeMaterial;
uniform Light light;



void main() {
	// ambient component
	vec3 ambient = light.ambient * vec3(texture(cubeMaterial.diffuse, vTexCoords));

	// diffuse component
	vec3 n = normalize(vNormal);
	vec3 l = normalize(light.position - vPos);
	float diff = max(dot(n, l), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(cubeMaterial.diffuse, vTexCoords));

	// specular component
	vec3 v = normalize(viewPos - vPos);
	vec3 r = reflect(-l, n);
	float spec = pow(max(dot(v, r), 0.0), cubeMaterial.shininess);
	vec3 specular = spec * light.specular * vec3(texture(cubeMaterial.specular, vTexCoords));

	// emission
	vec3 emission = vec3(texture(cubeMaterial.emission, vTexCoords)).ggb * cubeMaterial.emissionStrength;

	// result
	vec3 result = ambient + diffuse + specular + emission;
	fColor = vec4(result, 1.0);
}