#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nrm;
layout(location = 2) in vec2 _uv;
layout(location = 3) in ivec4 bone_ids;
layout(location = 4) in vec4 weights;
struct normal_map_t {
	sampler2D tex;
	bool exists;
};
uniform normal_map_t normal_map;
uniform mat4 model;
uniform mat4 model_transform;
uniform mat4 view;
uniform mat4 projection;
uniform bool has_bones;
uniform mat4 bones[100];
out vec2 uv;
out vec3 fragpos;
out vec3 normal;
void main() {
	vec4 position = vec4(pos, 1.0);
	mat4 transform = model * model_transform;
	if (has_bones) {
		mat4 bone_transform = bones[bone_ids[0]] * weights[0];
		bone_transform += bones[bone_ids[1]] * weights[1];
		bone_transform += bones[bone_ids[2]] * weights[2];
		bone_transform += bones[bone_ids[3]] * weights[3];
		position = bone_transform * position;
		transform = transform * bone_transform;
	}
	fragpos = vec3(transform * vec4(pos, 1.0));
	vec3 normal_value = nrm;
	if (normal_map.exists) {
		normal_value *= vec3(texture(normal_map.tex, _uv));
	}
	normal = mat3(transpose(inverse(transform))) * normal_value;
	gl_Position = projection * view * model * model_transform * position;
	uv = _uv;
}