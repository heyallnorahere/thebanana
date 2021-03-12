#version 410 core
in vec4 fragpos;
uniform vec3 lightpos;
uniform float far_plane;
void main() {
	float light_distance = length(fragpos.xyz - lightpos) / far_plane;
	gl_FragDepth = light_distance;
}