#include "mandel.h"


static void	st_zoom(State *state, bool zoom_in);
static void	st_move_horizontal(State *state, bool move_right);
static void	st_move_vertical(State *state, bool move_down);
static void	st_set_key(SDL_Keycode sym, bool value);
static void	st_apply_keys(State *state);

static bool	g_key_states[] = {
	[KEY_UP] = false,
	[KEY_DOWN] = false,
	[KEY_RIGHT] = false,
	[KEY_LEFT] = false,

	[KEY_INC_ITERATIONS] = false,
	[KEY_DEC_ITERATIONS] = false,

	[KEY_ZOOM_IN] = false,
	[KEY_ZOOM_OUT] = false,
};

void 		event_handle(State *state)
{
    SDL_Event	e;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                state->running = false;
                break;

            case SDL_KEYDOWN:
				st_set_key(e.key.keysym.sym, true);
                break;

            case SDL_KEYUP:
				st_set_key(e.key.keysym.sym, false);
                break;

            case SDL_MOUSEWHEEL:
                if (e.wheel.y == -1)
					st_zoom(state, true);
                else if (e.wheel.y == 1)
					st_zoom(state, false);
                break;

			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					double w_ratio = (double)state->width / (double)e.window.data1;
					double h_ratio = (double)state->height / (double)e.window.data2;
					SDL_GL_GetDrawableSize(state->window, &state->width, &state->height);
					GL_CALL(glViewport(0, 0, state->width, state->height));
					state->real_start /= w_ratio;
					state->real_end /= w_ratio;
					state->imag_start /= h_ratio;
					state->imag_end /= h_ratio;
				}
				break;
        }
    }
	st_apply_keys(state);
}

static void	st_set_key(SDL_Keycode sym, bool value)
{
	switch (sym)
	{
		case SDLK_r: g_key_states[KEY_INC_ITERATIONS] = value; break;
		case SDLK_e: g_key_states[KEY_DEC_ITERATIONS] = value; break;

		case SDLK_UP:
		case SDLK_k:
			g_key_states[KEY_UP] = value;
			break;
		case SDLK_DOWN:
		case SDLK_j:
			g_key_states[KEY_DOWN] = value;
			break;
		case SDLK_LEFT:
		case SDLK_h:
			g_key_states[KEY_LEFT] = value;
			break;
		case SDLK_RIGHT:
		case SDLK_l:
			g_key_states[KEY_RIGHT] = value;
			break;

		case SDLK_EQUALS:
		case SDLK_f:
			g_key_states[KEY_ZOOM_IN] = value;
			break;
		case SDLK_MINUS:
		case SDLK_d:
			g_key_states[KEY_ZOOM_OUT] = value;
			break;
	}
}

#define MANDEL_ITERATIONS_DELTA 2

static void	st_apply_keys(State *state)
{
	if (g_key_states[KEY_INC_ITERATIONS])
		state->iterations += MANDEL_ITERATIONS_DELTA;
	if (g_key_states[KEY_DEC_ITERATIONS])
	{
		state->iterations -= MANDEL_ITERATIONS_DELTA;
		if (state->iterations <= 0)
			state->iterations = 1;
	}

	if (g_key_states[KEY_UP])
		st_move_vertical(state, false);
	if (g_key_states[KEY_DOWN])
		st_move_vertical(state, true);
	if (g_key_states[KEY_LEFT])
		st_move_horizontal(state, false);
	if (g_key_states[KEY_RIGHT])
		st_move_horizontal(state, true);

	if (g_key_states[KEY_ZOOM_IN])
		st_zoom(state, true);
	if (g_key_states[KEY_ZOOM_OUT])
		st_zoom(state, false);
}

#define MANDEL_ZOOM_RATIO 64

static void	st_zoom(State *state, bool zoom_in)
{
	double factor = zoom_in ? 1 : -1;
	double real_change = (state->real_end - state->real_start) / MANDEL_ZOOM_RATIO;
	double imag_change = (state->imag_end - state->imag_start) / MANDEL_ZOOM_RATIO;
	state->real_start += factor * real_change;
	state->real_end -= factor * real_change;
	state->imag_start += factor * imag_change;
	state->imag_end -= factor * imag_change;
}

#define MANDEL_MOVE_RATIO 64

static void	st_move_horizontal(State *state, bool move_right)
{
	double factor = move_right ? 1 : -1;
	double real_change = (state->real_end - state->real_start) / MANDEL_MOVE_RATIO;
	state->real_start += factor * real_change;
	state->real_end += factor * real_change;
}

static void	st_move_vertical(State *state, bool move_down)
{
	double factor = move_down ? -1 : 1;
	double imag_change = (state->imag_end - state->imag_start) / MANDEL_MOVE_RATIO;
	state->imag_start += factor * imag_change;
	state->imag_end += factor * imag_change;
}
