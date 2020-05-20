#include "mandel.h"
#include "config.h"

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

static Color	color_hsl_to_rgb(ColorHSL color_hsl);
static Color	*st_hsl_rainbow(int count);
static int		st_compar_control_points(const void *ptr1, const void *ptr2);
static Color	*st_linear_iterpolation(int count, ControlPoint *points, size_t points_count);

unsigned int	color_texture_new(int count)
{
	unsigned int	texture;
	Color			*palette;

	if ((palette = st_hsl_rainbow(count)) == NULL)
		return 0;
	/* if ((palette = st_linear_iterpolation(count, g_theme, sizeof(g_theme) / sizeof(ControlPoint))) == NULL) */
	/* 	return 0; */
	GL_CALL(glGenTextures(1, &texture));
	GL_CALL(glBindTexture(GL_TEXTURE_1D, texture));
	GL_CALL(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_CALL(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GL_CALL(glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, count + 1, 0, GL_RGB, GL_UNSIGNED_BYTE, palette));
	free(palette);
	return texture;
}

static Color	*st_linear_iterpolation(int count, ControlPoint *points, size_t points_count)
{
	Color			*palette;

	if (points_count < 2)
		return NULL;
    if ((palette = malloc(sizeof(Color) * count)) == NULL)
		return NULL;
	qsort(points, points_count, sizeof(ControlPoint), st_compar_control_points);

	size_t	point_i = 0;

	double	x0 = points[point_i].position;
	double	x1 = points[point_i + 1].position;

	double	y0 = points[point_i].color.r;
	double	y1 = points[point_i + 1].color.r;

	double	delta_x = x1 - x0;
	double	delta_y = y1 - y0;

	double	m = delta_x / delta_y;
	double	b = y0 - x0;

	double red = 0;
	for (int i = 0; i < count; i++)
	{
		palette[i].r = (uint8_t)(255.0 * (m * red + b));
		red += 1.0 / (double)count;
		if (red >= x1)
		{
			if (point_i >= points_count)
				break;
			x0 = x1;
			point_i++;
			x1 = points[point_i + 1].position;
		}
		palette[i].g = 0;
		palette[i].b = 0;
		printf("%d\n", i);
	}
	return palette;
}

static int		st_compar_control_points(const void *ptr1, const void *ptr2)
{
	const ControlPoint	*point1 = ptr1;
	const ControlPoint	*point2 = ptr2;

	if (point1->position < point2->position)
		return -1;
	if (point1->position > point2->position)
		return 1;
	return 0;
}

static Color	*st_hsl_rainbow(int count)
{
	ColorHSL		hsl;
	Color			*palette;

    if ((palette = malloc(sizeof(Color) * count)) == NULL)
		return NULL;
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
	return palette;
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
