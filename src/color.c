#include "mandel.h"

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

static Color	color_hsl_to_rgb(ColorHSL color_hsl);

unsigned int	color_texture_new(int count)
{
	ColorHSL		hsl;
	Color			*palette;
	unsigned int	texture;

    palette = malloc(sizeof(Color) * count);
	if (palette == NULL)
		return 0;
    for (int i = 0; i < count; i++)
    {
		hsl.h = (uint8_t)(255.0 * ((double)i / (double)count));
		hsl.s = 150;
		hsl.l = 127;
		palette[i] = color_hsl_to_rgb(hsl);
    }
    for (int i = 0, j = count - 1; i < j; i++, j--)
	{
		Color tmp = palette[i];
		palette[i] = palette[j];
		palette[j] = tmp;
	}

	GL_CALL(glGenTextures(1, &texture));
	GL_CALL(glBindTexture(GL_TEXTURE_1D, texture));
	GL_CALL(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_CALL(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GL_CALL(glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, count + 1, 0, GL_RGB, GL_UNSIGNED_BYTE, palette));
	free(palette);
	return texture;
}

static Color	color_hsl_to_rgb(ColorHSL color_hsl)
{
	Color	color_rgb;
	double	h;
	double	s;
	double	l;
	double	r;
	double	g;
	double	b;
	double temp1, temp2, tempr, tempg, tempb;

	h = color_hsl.h / 256.0;
	s = color_hsl.s / 256.0;
	l = color_hsl.l / 256.0;
	if(s == 0)
	{
		r = l;
		g = l;
		b = l;
	}
	else
	{
		if (l < 0.5) temp2 = l * (1 + s);
		else temp2 = (l + s) - (l * s);
		temp1 = 2 * l - temp2;
		tempr = h + 1.0 / 3.0;
		if (tempr > 1) tempr--;
		tempg = h;
		tempb = h - 1.0 / 3.0;
		if (tempb < 0) tempb++;

		if (tempr < 1.0 / 6.0) r = temp1 + (temp2 - temp1) * 6.0 * tempr;
		else if (tempr < 0.5) r = temp2;
		else if (tempr < 2.0 / 3.0) r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
		else r = temp1;

		if (tempg < 1.0 / 6.0) g = temp1 + (temp2 - temp1) * 6.0 * tempg;
		else if (tempg < 0.5) g = temp2;
		else if (tempg < 2.0 / 3.0) g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
		else g = temp1;

		if(tempb < 1.0 / 6.0) b = temp1 + (temp2 - temp1) * 6.0 * tempb;
		else if(tempb < 0.5) b = temp2;
		else if(tempb < 2.0 / 3.0) b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
		else b = temp1;
	}
	color_rgb.r = (int)(r * 256.0);
	color_rgb.g = (int)(g * 256.0);
	color_rgb.b = (int)(b * 256.0);
	return (color_rgb);
}
