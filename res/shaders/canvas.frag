#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D cTex;

void main() {
	// FragColor = vec4(TexCoord, 0.0, 1.0);
	// FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	FragColor = texture(cTex, TexCoord);
}