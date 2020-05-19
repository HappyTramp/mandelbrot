#include "config.h"
#include "mandel.h"

bool	state_init(State *state)
{
    SDL_CALL(SDL_Init(SDL_INIT_VIDEO));
	SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
	SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0));
	SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
	SDL_CALL(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
    SDL_CALL(state->window = SDL_CreateWindow(
		MANDEL_WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		MANDEL_WINDOW_WIDTH,
		MANDEL_WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	));
	SDL_CALL(state->context = SDL_GL_CreateContext(state->window));
	assert(glewInit() == GLEW_OK);
	SDL_CALL(SDL_GL_SetSwapInterval(1));
	if (!shader_init(&state->shader))
	{
		perror(NULL);
		return false;
	}
	SDL_GL_GetDrawableSize(state->window, &state->width, &state->height);
	GL_CALL(glViewport(0, 0, state->width, state->height));

	/* state->palette = color_palette_new(NULL, MANDEL_ITERATIONS); */
	/* if (state->palette == NULL) */
	/* 	return false; */
	state->iterations = MANDEL_ITERATIONS;
	state->real_start = -2.0;
	state->real_end = 2.0;
	state->imag_start = -2.0;
	state->imag_end = 2.0;
    state->running = true;
    return true;
}

void	state_run(State *state)
{
    while (state->running)
    {
        event_handle(state);
		glClearColor(0.2, 0.3, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(state->window);
		SDL_Delay(3);
    }
}

void	state_quit(State *state)
{
    /* free(state->palette); */
	GL_CALL(glDeleteProgram(state->shader.id));
	SDL_GL_DeleteContext(state->context);
    SDL_DestroyWindow(state->window);
	SDL_Quit();
}
