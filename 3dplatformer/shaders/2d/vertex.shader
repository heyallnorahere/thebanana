#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 _uv;
out vec2 uv;
void main() {
	uv = _uv;
	gl_Position = vec4(pos, 1.0);
}