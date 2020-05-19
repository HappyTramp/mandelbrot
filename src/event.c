#include "mandel.h"

static void	st_zoom(State *state, bool zoom_in);
static void	st_move_horizontal(State *state, bool move_right);
static void	st_move_vertical(State *state, bool move_down);

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
                switch (e.key.keysym.sym)
                {
					case SDLK_r:
						state->iterations += 10;
						/* state->palette = color_palette_new(state->palette, state->iterations); */
						break;
					case SDLK_e:
						state->iterations -= 10;
						if (state->iterations <= 0)
							state->iterations = 1;
						/* state->palette = color_palette_new(state->palette, state->iterations); */
						break;
                    case SDLK_UP:
                    case SDLK_k:
						st_move_vertical(state, false);
                        break;
                    case SDLK_DOWN:
                    case SDLK_j:
						st_move_vertical(state, true);
                        break;
                    case SDLK_LEFT:
                    case SDLK_h:
						st_move_horizontal(state, false);
                        break;
                    case SDLK_RIGHT:
                    case SDLK_l:
						st_move_horizontal(state, true);
                        break;
                    case SDLK_EQUALS:
                    case SDLK_f:
						st_zoom(state, true);
                        break;
                    case SDLK_MINUS:
                    case SDLK_d:
						st_zoom(state, false);
                        break;
                    case SDLK_q:
                        state->running = false;
                }
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
					SDL_GL_GetDrawableSize(state->window, &state->width, &state->height);
					GL_CALL(glViewport(0, 0, state->width, state->height));
					/* double w_ratio = (double)width / (double)e.window.data1; */
					/* double h_ratio = (double)height / (double)e.window.data2; */
                    /*  */
					/* state->real_start /= w_ratio; */
					/* state->real_end /= w_ratio; */
					/* state->imag_start /= h_ratio; */
					/* state->imag_end /= h_ratio; */
				}
				break;
        }
    }
}

#define MANDEL_ZOOM_RATIO 30

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

#define MANDEL_MOVE_RATIO 20

static void	st_move_horizontal(State *state, bool move_right)
{
	double factor = move_right ? 1 : -1;
	double real_change = (state->real_end - state->real_start) / MANDEL_MOVE_RATIO;
	state->real_start += factor * real_change;
	state->real_end += factor * real_change;
}

static void	st_move_vertical(State *state, bool move_down)
{
	double factor = move_down ? 1 : -1;
	double imag_change = (state->imag_end - state->imag_start) / MANDEL_MOVE_RATIO;
	state->imag_start += factor * imag_change;
	state->imag_end += factor * imag_change;
}
