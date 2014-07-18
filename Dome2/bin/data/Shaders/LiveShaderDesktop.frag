#version 120
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect 	u_tex_unit0;
uniform float 			u_time;

float d = sin(u_time * 5.0)*0.5 + 1.5; // kernel offset

float lookup(vec2 p, float dx, float dy)
{
    vec2 uv = (p.xy + vec2(dx * d, dy * d)) ;
    vec4 c = texture2DRect(u_tex_unit0, uv.xy);
	
	// return as luma
    return 0.2126*c.r + 0.7152*c.g + 0.0722*c.b;
}

void main(void)
{
    vec2 p = gl_TexCoord[0].st;
    
	// simple sobel edge detection
    float gx = 0.0;
    gx += -1.0 * lookup(p, -1.0, -1.0);
    gx += -2.0 * lookup(p, -1.0,  0.0);
    gx += -1.0 * lookup(p, -1.0,  1.0);
    gx +=  1.0 * lookup(p,  1.0, -1.0);
    gx +=  2.0 * lookup(p,  1.0,  0.0);
    gx +=  1.0 * lookup(p,  1.0,  1.0);
    
    float gy = 0.0;
    gy += -1.0 * lookup(p, -1.0, -1.0);
    gy += -2.0 * lookup(p,  0.0, -1.0);
    gy += -1.0 * lookup(p,  1.0, -1.0);
    gy +=  1.0 * lookup(p, -1.0,  1.0);
    gy +=  2.0 * lookup(p,  0.0,  1.0);
    gy +=  1.0 * lookup(p,  1.0,  1.0);
    
	// hack: use g^2 to conceal noise in the video
    float g = gx*gx + gy*gy;
    float g2 = g * (sin(u_time) / 2.0 + 0.5);
    
    vec4 col = texture2DRect(u_tex_unit0, p );
    col += vec4(g, g2, 0., 1.0);
    // if(col.r>0)
    // {
    gl_FragColor = col;
    // }
    // else
    // {
     // gl_FragColor = vec4(0.5,0.5,0.5,1.);   
    // }
}