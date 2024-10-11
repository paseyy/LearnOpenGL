#version 330 core
in vec3 vColor;
in vec3 vPos;

out vec4 fColor;

void main() {
	fColor = vec4(vPos.xyz, 1.0);
}