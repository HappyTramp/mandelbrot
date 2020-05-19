#include "mandel.h"

void	error_check_sdl(const char *code, const char *filename, int line_num)
{
	const char	*err;

	err = SDL_GetError();
	if (*err == '\0')
		return ;
	SDL_Log("[ERROR SDL] %s\n\t(%s) at %s:%d\n", err, code, filename, line_num);
	SDL_Quit();
	exit(EXIT_FAILURE);
}

void	error_clear_gl(void)
{
	while (glGetError() != GL_NO_ERROR)
		;
}

void	error_check_gl(const char *code, const char *filename, int line_num)
{
	GLenum	err;
	bool	occured;
	char	*err_str = "";

	occured = false;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		switch (err)
		{
			case GL_INVALID_OPERATION:
				err_str = "GL_INVALID_OPERATION";
				break;
			case GL_INVALID_VALUE:
				err_str = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_ENUM:
				err_str = "GL_INVALID_ENUM";
				break;
		}
		fprintf(stderr, "[ERROR OPENGL] (%d) %s\n\t(%s) at %s:%d\n",
				err, err_str, code, filename, line_num);
		occured = true;
	}
	if (occured)
		exit(EXIT_FAILURE);
}
