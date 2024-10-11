#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float offsetX;

out vec3 vColor;
out vec3 vPos;

void main() {
	vPos = vec3(aPos.x + offsetX, -aPos.y, aPos.z);
	gl_Position = vec4(vPos, 1.0);

	vColor = aColor;
}