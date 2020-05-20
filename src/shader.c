#include "mandel.h"

#define MANDEL_SHADER_VERT_FILE "shader/vertex.glsl"
#define MANDEL_SHADER_FRAG_FILE "shader/fragment.glsl"

static unsigned int	st_compile(char *filepath, unsigned int type);
static int			st_get_location(unsigned int shader_id, const char *name);

bool				shader_init(Shader *shader)
{
	unsigned int	shader_vert;
	unsigned int	shader_frag;

	if ((shader_vert = st_compile(MANDEL_SHADER_VERT_FILE, GL_VERTEX_SHADER)) == 0
		|| (shader_frag = st_compile(MANDEL_SHADER_FRAG_FILE, GL_FRAGMENT_SHADER)) == 0)
		return false;

	GL_CALL(shader->id = glCreateProgram());
	GL_CALL(glAttachShader(shader->id, shader_vert));
	GL_CALL(glAttachShader(shader->id, shader_frag));
	GL_CALL(glLinkProgram(shader->id));
	GL_CALL(glValidateProgram(shader->id));
	GL_CALL(glDeleteShader(shader_vert));
	GL_CALL(glDeleteShader(shader_frag));

	if ((shader->location.width = st_get_location(shader->id, "u_width")) == -1
		|| (shader->location.height = st_get_location(shader->id, "u_height")) == -1
		|| (shader->location.real_start = st_get_location(shader->id, "u_real_start")) == -1
		|| (shader->location.real_end = st_get_location(shader->id, "u_real_end")) == -1
		|| (shader->location.imag_start = st_get_location(shader->id, "u_imag_start")) == -1
		|| (shader->location.imag_end = st_get_location(shader->id, "u_imag_end")) == -1
		|| (shader->location.iterations = st_get_location(shader->id, "u_iterations")) == -1
		|| (shader->location.texture = st_get_location(shader->id, "u_texture")) == -1)
		return false;
	return true;
}

static int			st_get_location(unsigned int shader_id, const char *name)
{
	int	location;

	GL_CALL(location = glGetUniformLocation(shader_id, name));
	return location;
}

void				shader_set_uniforms(Shader *shader, State *state)
{
	GL_CALL(glUniform1i(shader->location.width, state->width));
	GL_CALL(glUniform1i(shader->location.height, state->height));

	GL_CALL(glUniform1f(shader->location.real_start, state->real_start));
	GL_CALL(glUniform1f(shader->location.real_end, state->real_end));
	GL_CALL(glUniform1f(shader->location.imag_start, state->imag_start));
	GL_CALL(glUniform1f(shader->location.imag_end, state->imag_end));

	GL_CALL(glUniform1i(shader->location.iterations, state->iterations));

	GL_CALL(glUniform1i(shader->location.texture, 0));
	GL_CALL(glActiveTexture(GL_TEXTURE0));
	GL_CALL(glBindTexture(GL_TEXTURE_1D, state->texture));
}

static unsigned int	st_compile(char *filepath, unsigned int type)
{
	unsigned int	id;
	int				result;
	FILE			*file;
	char			*source;

	if ((file = fopen(filepath, "r")) == NULL)
		return 0;
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	rewind(file);

	if ((source = malloc(sizeof(char) * (file_size + 1))) == NULL
		|| fread(source, sizeof(char), file_size, file) != (unsigned long)file_size)
	{
		free(source);
		fclose(file);
		return 0;
	}
	fclose(file);
	source[file_size] = '\0';

	GL_CALL(id = glCreateShader(type));
	GL_CALL(glShaderSource(id, 1, (const char**)&source, (int*)&file_size));
	free(source);
	GL_CALL(glCompileShader(id));

	GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int		len;
		char	*msg;
		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
		if ((msg = malloc(sizeof(char) * (len + 1))) == NULL)
			return 0;
		GL_CALL(glGetShaderInfoLog(id, len, &len, msg));
		fputs(msg, stderr);
		free(msg);
		GL_CALL(glDeleteShader(id));
		return 0;
	}
	return id;
}
