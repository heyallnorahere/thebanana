#version 410 core
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
	int type;
	vec3 position;
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float ambient_strength;
	float cutoff;
	samplerCube depthmap_cube;
	sampler2D depthmap_2d;
};
uniform material_t shader_material;
uniform light lights[7];
uniform mat4 light_space_matrices[30];
uniform int light_count;
uniform vec3 viewpos;
uniform float far_plane;
in vec3 fragpos;
in vec3 normal;
float calculate_point_shadows(int light_index) {
	vec3 frag_to_light = fragpos - lights[light_index].position;
	float closest_depth = texture(lights[light_index].depthmap_cube, frag_to_light).r * far_plane;
	float current_depth = length(frag_to_light);
	float bias = 0.05;
	float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;
	return shadow;
}
vec3 calculate_point_light(int light_index, vec3 nrm) {
	vec3 ambient = lights[light_index].ambient * lights[light_index].ambient_strength * shader_material.ambient;
	vec3 light_dir = normalize(lights[light_index].position - fragpos);
	float diff = max(dot(nrm, light_dir), 0.0);
	vec3 diffuse = diff * lights[light_index].diffuse * shader_material.diffuse;
	vec3 view_dir = normalize(viewpos - fragpos);
	vec3 reflect_dir = reflect(-light_dir, nrm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shader_material.shininess);
	vec3 specular = lights[light_index].specular * (spec * shader_material.specular);
	float shadow = calculate_point_shadows(light_index);
	return (ambient + (1.0 - shadow) * (diffuse + specular));
}
float calculate_shadows(int light_index, vec3 nrm, vec3 light_dir) {
	vec4 fragpos_light_space = light_space_matrices[light_index] * vec4(fragpos, 1.0);
	vec3 proj_coords = fragpos_light_space.xyz / fragpos_light_space.w;
	proj_coords = proj_coords * 0.5 + 0.5;
	float closest_depth = texture(lights[light_index].depthmap_2d, proj_coords.xy).r;
	float current_depth = proj_coords.z;
	float bias = max(0.05 * (1.0 - dot(nrm, light_dir)), 0.005);
	float shadow = 0.0;
	vec2 texel_size = 1.0 / textureSize(lights[light_index].depthmap_2d, 0);
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float pcf_depth = texture(lights[light_index].depthmap_2d, proj_coords.xy + vec2(x, y) * texel_size).r;
			shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	if (proj_coords.z > 1.0) {
		shadow = 0.0;
	}
	return shadow;
}
vec3 calculate_directional_light(int light_index, vec3 nrm) {
	vec3 ambient = lights[light_index].ambient * lights[light_index].ambient_strength * shader_material.ambient;
	vec3 light_dir = normalize(-lights[light_index].direction);
	float diff = max(dot(nrm, light_dir), 0.0);
	vec3 diffuse = diff * lights[light_index].diffuse * shader_material.diffuse;
	vec3 view_dir = normalize(viewpos - fragpos);
	vec3 reflect_dir = reflect(-light_dir, nrm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shader_material.shininess);
	vec3 specular = lights[light_index].specular * (spec * shader_material.specular);
	float shadow = calculate_shadows(light_index, nrm, light_dir);
	return (ambient + (1.0 - shadow) * (diffuse + specular));
}
vec3 calculate_spotlight(int light_index, vec3 nrm) {
	vec3 light_dir = normalize(lights[light_index].position - fragpos);
	float theta = dot(light_dir, normalize(-lights[light_index].direction));
	if (theta > lights[light_index].cutoff) {
		vec3 ambient = lights[light_index].ambient * lights[light_index].ambient_strength * shader_material.ambient;
		float diff = max(dot(nrm, light_dir), 0.0);
		vec3 diffuse = diff * lights[light_index].diffuse * shader_material.diffuse;
		vec3 view_dir = normalize(viewpos - fragpos);
		vec3 reflect_dir = reflect(-light_dir, nrm);
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shader_material.shininess);
		vec3 specular = lights[light_index].specular * (spec * shader_material.specular);
		float shadow = calculate_shadows(light_index, nrm, light_dir);
		return (ambient + (1.0 - shadow) * (diffuse + specular));
	}
	return vec3(0, 0, 0);
}
void main() {
	vec3 color = vec3(0, 0, 0);
	vec3 nrm = normalize(normal);
	for (int i = 0; i < light_count; i++) {
		vec3 c = vec3(0, 0, 0);
		switch (lights[i].type) {
		case 1:
			c = calculate_point_light(i, nrm);
			break;
		case 2:
			c = calculate_directional_light(i, nrm);
			break;
		case 3:
			c = calculate_spotlight(i, nrm);
			break;
		}
		color += c;
	}
	fragment_color = texture(shader_material.tex, uv) * vec4(color, 1.0);
}