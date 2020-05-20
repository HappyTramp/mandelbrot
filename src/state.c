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

	float vertices[] = {
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,

		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,
	};
	GL_CALL(glGenVertexArrays(1, &state->vertex_array));
	GL_CALL(glBindVertexArray(state->vertex_array));

	GL_CALL(glGenBuffers(1, &state->vertex_buf));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, state->vertex_buf));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));

	state->iterations = MANDEL_ITERATIONS;
	state->texture = color_texture_new(1024);
	if (state->texture == 0)
		return false;
	state->real_start = -2.0;
	state->real_end = 2.0;
	state->imag_start = -2.0;
	state->imag_end = 2.0;
    state->running = true;
	state->smooth = false;
	state->samples = 1.0;
    return true;
}

void	state_run(State *state)
{
    while (state->running)
    {
        event_handle(state);
		GL_CALL(glClearColor(0.2, 0.3, 0.2, 1.0));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		GL_CALL(glUseProgram(state->shader.id));
		shader_set_uniforms(&state->shader, state);
		GL_CALL(glBindVertexArray(state->vertex_array));
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));

		SDL_GL_SwapWindow(state->window);
		SDL_Delay(3);
    }
}

void	state_quit(State *state)
{
	GL_CALL(glDeleteTextures(1, &state->texture));
	GL_CALL(glDeleteBuffers(1, &state->vertex_buf));
	GL_CALL(glDeleteVertexArrays(1, &state->vertex_array));
	GL_CALL(glDeleteProgram(state->shader.id));
	SDL_GL_DeleteContext(state->context);
    SDL_DestroyWindow(state->window);
	SDL_Quit();
}
