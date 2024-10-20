#version 330 core
in vec3 vNormal;
in vec3 vPos;

out vec4 fColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	// ambient component
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse component
	float diffuseStrength = 0.3;
	vec3 n = normalize(vNormal);
	vec3 l = normalize(lightPos - vPos);
	float diff = max(dot(n, l), 0.0) * diffuseStrength;
	vec3 diffuse = diff * lightColor;

	// specular component
	float specularStrength = 0.8;
	vec3 v = normalize(viewPos - vPos);
	vec3 r = reflect(-l, n);
	float spec = pow(max(dot(v, r), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// result
	vec3 result = (ambient + diffuse + specular) * objectColor;
	fColor = vec4(result, 1.0);
}