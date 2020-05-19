#ifndef MANDEL_H
# define MANDEL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include <GL/glew.h>
# include <SDL2/SDL.h>

# include <assert.h>

#define SDL_CALL(x) do {                     \
	SDL_ClearError();                        \
	x;                                       \
	error_check_sdl(#x, __FILE__, __LINE__); \
} while (0)

# define GL_CALL(x) do {                    \
	error_clear_gl();                       \
	x;                                      \
	error_check_gl(#x, __FILE__, __LINE__); \
} while (0)

typedef union
{
    uint32_t		data;
    struct
    {
        uint8_t 	b;
        uint8_t 	g;
        uint8_t 	r;
    };
}					Color;

typedef struct
{
	uint8_t 	h;
	uint8_t 	s;
	uint8_t 	l;
}					ColorHSL;

typedef struct
{
    double			x;
    double			y;
}					Point;

typedef struct
{
	unsigned int	id;
	struct
	{
		int			width;
		int			height;

		float		real_start;
		float		real_end;
		float		imag_start;
		float		imag_end;

		int			iterations;
	}				location;
}					Shader;

typedef struct
{
    SDL_Window		*window;
	SDL_GLContext	context;
    bool			running;
	int				width;
	int				height;

	unsigned int	vertex_buf;
	unsigned int	vertex_array;
	unsigned int	texture;

	Shader			shader;

    // Color			*palette;

	double			real_start;
	double			real_end;
	double			imag_start;
	double			imag_end;
	int				iterations;
}					State;

// mandelbrot.c
int					mandelbrot(double ca, double cb, int iterations);

// state.c
bool				state_init(State *state);
void				state_quit(State *state);
void				state_run(State *state);

// event.c
void				event_handle(State *state);

// error.c
void				error_check_sdl(const char *code, const char *filename, int line_num);
void				error_clear_gl(void);
void				error_check_gl(const char *code, const char *filename, int line_num);

// color.c
Color				*color_palette_new(Color *palette, int iterations);

// shader.c
bool				shader_init(Shader *shader);
void				shader_set_uniforms(Shader *shader, State *state);

#endif
