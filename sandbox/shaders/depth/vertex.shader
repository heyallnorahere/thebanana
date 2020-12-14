#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 3) in ivec4 bone_ids;
layout(location = 4) in vec4 weights;
uniform mat4 model;
uniform mat4 model_transform;
uniform bool has_bones;
uniform mat4 bones[100];
void main() {
	vec4 position = vec4(pos, 1.0);
	if (has_bones) {
		mat4 bone_transform = bones[bone_ids[0]] * weights[0];
		bone_transform += bones[bone_ids[1]] * weights[1];
		bone_transform += bones[bone_ids[2]] * weights[2];
		bone_transform += bones[bone_ids[3]] * weights[3];
		position = bone_transform * position;
	}
	gl_Position = model * model_transform * position;
}