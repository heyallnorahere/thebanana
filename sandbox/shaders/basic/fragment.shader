#version 460 core
uniform sampler2D texture_diffuse1;
out vec4 fragment_color;
in vec2 uv;
uniform bool solid_color;
uniform vec3 fill_color;
void main() {
	if (solid_color) {
		fragment_color = vec4(fill_color, 1.0);
	} else {
		fragment_color = texture(texture_diffuse1, uv);
	}
}