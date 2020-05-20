#version 400 core

out vec4            out_color;

uniform int         u_width;
uniform int         u_height;

uniform float       u_real_start;
uniform float       u_real_end;
uniform float       u_imag_start;
uniform float       u_imag_end;

uniform int         u_iterations;
uniform bool        u_smooth;
uniform float       u_samples;

uniform sampler1D   u_texture;

#define ESCAPE_RADIUS 4.0
#define LOG_2 0.69314718056


int     mandelbrot_func(vec2 c)
{
    vec2    z;
    vec2    z_square;
    int     n;

    z = c;
    for (n = 0; n < u_iterations; n++)
    {
        z_square = z * z;
        if (z_square.x + z_square.y > ESCAPE_RADIUS)
            break;
        z.y = 2.0 * z.x * z.y;
        z.x = z_square.x - z_square.y;
        z += c;
    }
    return n;
}

float   mandelbrot_smooth(vec2 c)
{
    vec2    z;
    vec2    z_square;
    int     n;

    z = c;
    for (n = 0; n < u_iterations; n++)
    {
        z_square = z * z;
        if (z_square.x + z_square.y > ESCAPE_RADIUS)
            break;
        z.y = 2.0 * z.x * z.y;
        z.x = z_square.x - z_square.y;
        z += c;
    }
    if (n == u_iterations)
        return float(n);
    // http://linas.org/art-gallery/escape/escape.html
    z_square = z * z;
    z.y = 2.0 * z.x * z.y;
    z.x = z_square.x - z_square.y;
    z += c;
    z_square = z * z;
    z.y = 2.0 * z.x * z.y;
    z.x = z_square.x - z_square.y;
    z += c;
    float modulus = sqrt(z.x * z.x + z.y * z.y);
    return float(n) - log(log(modulus)) / LOG_2;
}

vec4   mandelbrot_color(vec2 c)
{
    float   n;

    if (u_smooth)
        n = mandelbrot_smooth(c);
    else
        n = float(mandelbrot_func(c));

    if (n == float(u_iterations))
        return vec4(0.0, 0.0, 0.0, 1.0);
    else
        return texture(u_texture, n / float(u_iterations));

}

vec4    supersample_grid(vec2 c)
{
    vec2    epsilon;
    vec2    _sample;
    vec2    _step;
    vec4    color;

    color = vec4(0.0, 0.0, 0.0, 0.0);
    _step.x = (u_real_end - u_real_start) / float(u_width);
    _step.y = (u_imag_end - u_imag_start) / float(u_height);
    epsilon.y = 0.0;
    while (epsilon.y < u_samples)
    {
        epsilon.x = 0.0;
        while (epsilon.x < u_samples)
        {
            _sample = c + _step * (epsilon / u_samples);
            color += mandelbrot_color(_sample);
            epsilon.x += 1.0;
        }
        epsilon.y += 1.0;
    }
    return color / (u_samples * u_samples);
}

void main()
{
    vec2    c;
    float   n;

    c.x = u_real_start + float(gl_FragCoord.x) / float(u_width) * (u_real_end - u_real_start);
    c.y = u_imag_start + float(gl_FragCoord.y) / float(u_height) * (u_imag_end - u_imag_start);


    if (u_samples == 1.0)
        out_color = mandelbrot_color(c);
    else
        out_color = supersample_grid(c);
}
