#include "mandel.h"

#define MANDEL_SHADER_VERT_FILE "shader/vertex.glsl"
#define MANDEL_SHADER_FRAG_FILE "shader/fragment.glsl"

static unsigned int	st_compile(char *filepath, unsigned int type);

bool				shader_init(Shader *shader)
{
	unsigned int	shader_vert;
	unsigned int	shader_frag;

	shader_vert = st_compile(MANDEL_SHADER_VERT_FILE, GL_VERTEX_SHADER);
	if (shader_vert == 0)
		return false;
	shader_frag = st_compile(MANDEL_SHADER_FRAG_FILE, GL_FRAGMENT_SHADER);
	if (shader_frag == 0)
		return false;

	GL_CALL(shader->id = glCreateProgram());
	GL_CALL(glAttachShader(shader->id, shader_vert));
	GL_CALL(glAttachShader(shader->id, shader_frag));
	GL_CALL(glLinkProgram(shader->id));
	GL_CALL(glValidateProgram(shader->id));
	GL_CALL(glDeleteShader(shader_vert));
	GL_CALL(glDeleteShader(shader_frag));
	return (true);
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
