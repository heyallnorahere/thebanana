#version 460 core
uniform sampler2D texture_diffuse1;
out vec4 fragment_color;
in vec2 uv;
uniform bool solid_color;
uniform vec3 fill_color;
struct material_t {
	sampler2D albedo;
	vec3 color;
	float shininess;
};
uniform material_t material;
void main() {
	if (solid_color) {
		fragment_color = vec4(fill_color, 1.0);
	} else {
		vec4 tex = texture(material.albedo, uv);
		fragment_color = vec4(vec3(tex) * material.color, tex.a);
	}
}