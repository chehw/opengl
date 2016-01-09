#ifndef _SHADERS_IMPL_H_
#define _SHADERS_IMPL_H_


static const GLchar* VertexShader =
{
  "#version 330\n"\

  "layout(location=0) in vec4 in_Position;\n"\
  "layout(location=1) in vec4 in_Color;\n"\
  "out vec4 ex_Color;\n"\

  "void main(void)\n"\
  "{\n"\
  "  gl_Position = in_Position;\n"\
  "  ex_Color = in_Color;\n"\
  "}\n"
};

static const GLchar* FragmentShader =
{
  "#version 330\n"\

  "in vec4 ex_Color;\n"\
  "out vec4 out_Color;\n"\

  "void main(void)\n"\
  "{\n"\
  "  out_Color = ex_Color;\n"\
  "}\n"
};


static void shaders_create()
{
	GLenum err_code = glGetError();
	
	gl.vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(gl.vertex_shader_id, 1, &VertexShader, NULL);
	glCompileShader(gl.vertex_shader_id);
	
	gl.fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(gl.fragment_shader_id, 1, &FragmentShader, NULL);
	glCompileShader(gl.fragment_shader_id);
	
	gl.program_id = glCreateProgram();
		glAttachShader(gl.program_id, gl.vertex_shader_id);
		glAttachShader(gl.program_id, gl.fragment_shader_id);
	glLinkProgram(gl.program_id);
	glUseProgram(gl.program_id);
	
	gl_check_error(err_code);
	
}

static void shaders_destroy()
{
	GLenum err_code = glGetError();
	
	glUseProgram(0);
	glDetachShader(gl.program_id, gl.vertex_shader_id);
	glDetachShader(gl.program_id, gl.fragment_shader_id);
	
	glDeleteShader(gl.vertex_shader_id);
	glDeleteShader(gl.fragment_shader_id);
	
	glDeleteProgram(gl.program_id);
	
	gl_check_error(err_code);
}

#endif
