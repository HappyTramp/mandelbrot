#ifndef CONFIG_H
# define CONFIG_H

# include "mandel.h"

# define MANDEL_WINDOW_WIDTH  400
# define MANDEL_WINDOW_HEIGHT 400
# define MANDEL_WINDOW_TITLE "Mandelbrot"
# define MANDEL_ITERATIONS 50

static ControlPoint g_theme[] = {
	{0.0,    {0x00, 0x0F, 0x64} },
	{0.16,   {0x20, 0x6B, 0xCB} },
	{0.42,   {0xED, 0xFF, 0xFF} },
	{0.6425, {0xFF, 0xAA, 0x00} },
	{1.0,    {0x00, 0x02, 0x00} },
	// {0.8575, {0x00, 0x02, 0x00} },
};

#endif
