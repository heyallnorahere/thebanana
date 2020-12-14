#version 460 core
out vec4 fragment_color;
in vec2 uv;
struct material_t {
	sampler2D tex;
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
};
struct light {
	vec3 position;
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float ambient_strength;
	samplerCube depthmap;
};
uniform material_t shader_material;
uniform light lights[100];
uniform int light_count;
uniform vec3 viewpos;
uniform float far_plane;
in vec3 fragpos;
in vec3 normal;
float calculate_shadows(int light_index) {
	vec3 frag_to_light = fragpos - lights[light_index].position;
	float closest_depth = texture(lights[light_index].depthmap, frag_to_light).r * far_plane;
	float current_depth = length(frag_to_light);
	float bias = 0.05;
	float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;
	return shadow;
}
void main() {
	vec3 color = vec3(0, 0, 0);
	vec3 nrm = normalize(normal);
	for (int i = 0; i < light_count; i++) {
		vec3 ambient = lights[i].ambient * lights[i].ambient_strength * shader_material.ambient;
		vec3 light_dir = normalize(lights[i].position - fragpos);
		float diff = max(dot(nrm, light_dir), 0.0);
		vec3 diffuse = diff * lights[i].diffuse * shader_material.diffuse;
		vec3 view_dir = normalize(viewpos - fragpos);
		vec3 reflect_dir = reflect(-light_dir, nrm);
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shader_material.shininess);
		vec3 specular = lights[i].specular * (spec * shader_material.specular);
		float shadow = calculate_shadows(i);
		color += (ambient + (1.0 - shadow) * (diffuse + specular));
	}
	fragment_color = texture(shader_material.tex, uv) * vec4(color, 1.0);
}