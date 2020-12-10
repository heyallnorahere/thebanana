#version 460 core
out vec4 fragment_color;
in vec2 uv;
struct material_t {
	sampler2D albedo;
	vec3 color;
	float shininess;
};
uniform material_t shader_material;
void main() {
	fragment_color = texture(shader_material.albedo, uv) * vec4(shader_material.color, 1.0);
}