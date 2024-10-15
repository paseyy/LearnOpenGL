#version 330 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D wood;
uniform sampler2D face;

void main() {
	fColor = mix(
		texture(wood, vTexCoord).rgba,
		texture(face, vTexCoord).rgba,
		texture(face, vTexCoord).a
	);
}