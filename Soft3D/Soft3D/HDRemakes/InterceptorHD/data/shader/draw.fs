#version 330

uniform sampler2D tex;
uniform vec4 col;

in vec2 uv;
in vec4 vcol;
out vec4 out_color;

void main() {

    vec4 rgb = texture(tex,uv) * vcol;


    out_color = rgb;

}