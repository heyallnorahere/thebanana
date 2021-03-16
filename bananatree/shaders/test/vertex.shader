#version 410
// feel free to edit this shader as you see fit, it isnt all that important
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
out vec3 interp_color;
void main() {
    gl_Position = vec4(pos, 1.0);
    interp_color = color;
}