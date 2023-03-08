#version 330
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec4 a_col;
layout (location = 3) in vec2 a_opos;
uniform mat4 projection;

out vec2 uv;
out vec4 vcol;
out vec2 ppos;


void main() {
    gl_Position = projection * vec4(a_position, 1.0);
    uv = a_uv;
    vcol = a_col;
    ppos = a_opos;

}
