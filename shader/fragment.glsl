#version 400 core

out vec4            out_color;

uniform int         u_width;
uniform int         u_height;

uniform float       u_real_start;
uniform float       u_real_end;
uniform float       u_imag_start;
uniform float       u_imag_end;

uniform int         u_iterations;

uniform sampler1D   u_texture;

#define ESCAPE_RADIUS 4.0

void main()
{
    float   ca = u_real_start + float(gl_FragCoord.x) / float(u_width) * (u_real_end - u_real_start);
    float   cb = u_imag_start + float(gl_FragCoord.y) / float(u_height) * (u_imag_end - u_imag_start);
    float	zr = ca;
    float	zi = cb;

    float	zr_square;
    float	zi_square;
    int     n;

    for (n = 0; n < u_iterations; n++)
    {
        zi_square = zi * zi;
        zr_square = zr * zr;
        if (zr_square + zi_square > ESCAPE_RADIUS)
            break;
        zi = 2.0 * zr * zi;
        zr = zr_square - zi_square;
        zi += cb;
        zr += ca;
    }

    if (n == u_iterations)
        out_color = vec4(0.0, 0.0, 0.0, 1.0);
    else
        out_color = texture1D(u_texture, float(n) / float(u_iterations));
}
