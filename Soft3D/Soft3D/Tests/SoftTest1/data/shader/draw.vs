#version 330
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec4 a_col;
uniform mat4 projection;

out vec2 uv;
out vec4 vcol;

void main() {
    gl_Position = projection * vec4(a_position, 1.0);
    uv = a_uv;
    vcol = a_col;
}