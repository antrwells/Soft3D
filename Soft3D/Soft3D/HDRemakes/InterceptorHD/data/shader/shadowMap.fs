#version 330

uniform sampler2D tex;
uniform sampler2D norm;
uniform sampler2D shadow;
uniform vec4 col;
uniform vec3 lightPos;
uniform vec3 lightDiff;
uniform float lightRange;
uniform vec3 screenSize;
uniform float mapRot;
uniform vec3 shadowSize;

in vec2 uv;
in vec4 vcol;
in vec2 ppos;

out vec4 out_color;

void main() {


    float sx = ppos.x/shadowSize.x;
    float sy = ppos.y/shadowSize.y;

    sy = 1.0 - sy;

    float lx = lightPos.x/shadowSize.x;
    float ly = lightPos.y/shadowSize.y;
    ly = 1.0 - ly;

    float xi = lx-sx;
    float yi = ly-sy;

    float shade = 1.0;

    float su,sv;

    su = sx;
    sv = sy;

    float cycle = 64;

    xi = xi / cycle;
    yi = yi / cycle;
    float cv = 0.0f;
    float samples =0;

    float iv = texture(shadow,vec2(sx,sy)).r;

    if(iv<0.1){

        shade = 1.0;

    }
else{
    for(int x=-2;x<3;x++){
    for(int y=-2;y<3;y++){
    
        float ax,ay;
        ax = x * 0.0008;
        ay = y * 0.0008;
        //su =sx + (x)*0.01;
       // sv =sy + (y)*0.01;
       su = sx + ax;
       sv = sy + ay;
        cv = 1.0;
            for(int i=0;i<cycle;i++){
            
                float v = texture(shadow,vec2(su,sv)).r;

                su = su + xi;
                sv = sv + yi;


                if(v<0.9)
                {
                cv = 0;
                break;
                }

            }

            shade = shade+cv;
            samples++;

        }
    }

    shade = shade / samples;
}

//    float shade = texture(shadow,vec2(sx,sy)).r;



    float xd,yd;

    
    xd = lightPos.x - ppos.x;
    yd = lightPos.y - ppos.y;
    float dis = sqrt(xd*xd+yd*yd);

    dis = dis / lightRange;
    if(dis>1.0) dis = 1.0;
    dis = 1.0 - dis;

    vec3 surfacePos = vec3(ppos.x,ppos.y,0.0);

    vec3 surfaceNormal = texture(norm,uv).rgb;
    surfaceNormal = -1 +surfaceNormal *2.0f;

    float nx,ny;

    //nx = surfaceNormal.x * cos(mapRot) - surfaceNormal.y * sin(mapRot);
   // ny = surfaceNormal.x * sin(mapRot) + surfaceNormal.y * cos(mapRot);

 //   surfaceNormal.x = nx;
 //   surfaceNormal.y = ny;
    surfaceNormal.z =-surfaceNormal.z;

    vec3 lightDir = lightPos - surfacePos;

    lightDir.x=-lightDir.x;
    lightDir.y=-lightDir.y;
    lightDir = normalize(-lightDir);
    vec3 normal = surfaceNormal;

   
    float diff = max(dot(lightDir,normal),0);
    
    vec4 rgb = texture(tex,uv)*  vcol * (dis * diff);
    rgb.rgb = rgb.rgb * lightDiff * shade; ;// * dis * diff;


    
    //rgb.rgb = texture(norm,uv).rgb;
    rgb.a = texture(tex,uv).a;
    if(rgb.a<0.05)
    {
        discard;
    }


    out_color = rgb;

}