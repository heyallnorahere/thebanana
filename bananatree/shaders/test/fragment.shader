#version 410 core
out vec4 fragment_color;
in vec3 interp_color;
void main() {
    fragment_color = vec4(interp_color, 1.0);
}