#version 330 core
in vec3 vColor;
in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D tWood;
uniform sampler2D tFace;

uniform float faceAlpha;

void main() {
	vec2 vTexCoordYFlipped = vec2(1.0 - vTexCoord.x, vTexCoord.y);
	fColor = mix(
		texture(tWood, vTexCoord / 2).rgba,
		texture(tFace, vTexCoordYFlipped).rgba, 
		vec4(texture(tFace, vTexCoord)).a * faceAlpha
	);
}