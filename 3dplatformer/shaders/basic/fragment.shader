#version 460 core
uniform sampler2D texture_diffuse1;
out vec4 fragment_color;
in vec2 uv;
uniform float x;
uniform float y;
void main() {
	/*float normalized_x = x + 0.5;
	float normalized_y = y + 0.5;
	bool _x = (normalized_x - 0.1 < uv.x && normalized_x + 0.1 > uv.x);
	bool _y = (normalized_y - 0.1 < uv.y && normalized_y + 0.1 > uv.y);
	if (_x && _y) {
		fragment_color = vec4(0.0, 1.0, 0.0, 1.0);
	} else {
		fragment_color = vec4(1.0, 0.0, 0.0, 1.0);
	}*/
	fragment_color = texture(texture_diffuse1, uv);
}