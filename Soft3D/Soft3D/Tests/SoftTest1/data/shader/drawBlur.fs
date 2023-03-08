#version 330

uniform sampler2D tex;
uniform vec4 col;
uniform float blur;

in vec2 uv;
in vec4 vcol;
out vec4 out_color;

void main() {

    //vec4 rgb = texture(tex,uv) * vcol;

    //rgb.rgb = vec3(1,1,1);
    //rgb.a = 1.0;
    vec3 col = vec3(0,0,0);
    float samples =0;

    for(int y=-7;y<7;y++){

        for(int x=-7;x<7;x++){

            vec2 tc = uv;
            tc.x = tc.x + x * blur;
            tc.y = tc.y + y * blur;
            if(tc.x>=0 && tc.x<=1 && tc.y>=0 && tc.y<=1.0)
            {
            col = col + texture(tex,tc).rgb;
            samples++;
            }
        }

    }

    col = col / samples;

    vec4 rgb;
    rgb.rgb = col;
    rgb.a = vcol.a;
    

    out_color = rgb;

}