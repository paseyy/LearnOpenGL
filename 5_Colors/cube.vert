#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

out vec3 vNormal;
out vec3 vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
	
void main() {
	gl_Position = projection * view * model * vec4(inPos, 1.0);

	vPos = vec3(model * vec4(inPos, 1.0));
	vNormal = mat3(transpose(inverse(model))) * inNormal;
}