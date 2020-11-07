#version 460 core
uniform sampler2D tex;
in vec2 uv;
out vec4 fragColor;
void main() {
	fragColor = texture(tex, uv);
}