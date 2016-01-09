/*
 * chapter-3.2.c
 * 
 * origin: https://github.com/openglbook/openglbook-samples/tree/master/chapter-3
 * modified by:  Che Hongwei <htc.chehw@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#define _DEBUG_INFO

#include <stdio.h>
#include <stdlib.h>
//~ #include <GL/glew.h>
//~ #include <GL/gl.h>
//~ #include <GL/freeglut.h>
#include "ch_util_gl.h"

// utilities
static void init(int argc, char ** argv);
static void cleanup();
static void init_window();
static void vbo_create();
static void vbo_destroy();

// callbacks
static void on_size(int width, int height);
static void on_draw(void);
static void on_close(void);
static void on_key_press(int key, int, int);
static void on_timer(int id);
static void on_idle(void);

struct MY_GL_CONTEXT
{
	int window;
	int height;
	int width;
	int show_mode;
	unsigned int frame_count;
	GLuint vao_id;
	GLuint vbo_id;
	GLuint index_buffer_id;
	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;
} gl;


typedef struct Vertex
{
	float xyzw[4];
	float rgba[4];
}Vertex_t;


#define SCREEN_WIDTH 	800
#define SCREEN_HEIGHT 	600
const char app_name[] = "chapter-2.4";

#include "shaders_impl.h"
int main(int argc, char **argv)
{
	setenv("DRI_PRIME", "1", 1);
	init(argc, argv);	
	glutMainLoop();
	return 0;
}

static void init(int argc, char ** argv)
{
	info("init");
	glutInit(&argc, argv);
	GLenum err_code = glGetError();	
	if(err_code != GL_NO_ERROR) gl_err_exit(err_code);
	
	// init context
	glutInitContextVersion(GLUT_CONTEXT_MAJOR_VERSION, GLUT_CONTEXT_MINOR_VERSION);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	init_window();
	
	// init glew
	glewExperimental = GL_TRUE;
	err_code = glewInit();
	if(GLEW_OK != err_code)
	{
		fprintf(stderr, "ERROR: %s\n", glewGetErrorString(err_code));
		exit(-1);
	}
	
	info("OpenGL Version: %s", glGetString(GL_VERSION));	
	
	shaders_create();
	vbo_create();
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
}

static void init_window()
{
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	
	gl.window = glutCreateWindow(app_name);
	if(gl.window < 1) gl_err_exit(glGetError());
	
	// register callbacks
	glutReshapeFunc(on_size);
	glutDisplayFunc(on_draw);
	glutCloseFunc(on_close);
	glutIdleFunc(on_idle);
	glutTimerFunc(0, on_timer, 0);
	glutKeyboardFunc((void (*)(unsigned char, int, int))on_key_press);
	glutSpecialFunc(on_key_press);
	
	
}


// callbacks
static void on_size(int width, int height)
{
	gl.width = width;
	gl.height = height;
	glViewport(0, 0, width, height);
}

static void on_draw(void)
{
	++gl.frame_count;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//~ glDrawArrays(GL_TRIANGLES, 0, 3);
	//~ glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, (GLvoid *)0);
	long offset = 0;
	long count = 48;
	
	switch(gl.show_mode)
	{
	case 0: offset = 0; count = 48; break;	
	case 1: offset = 0; count = 12; break;
	case 2: offset = 12; count = 12; break;
	case 3: offset = 24; count = 12; break;
	case 4: offset = 36; count = 12; break;
	default: 		
		break;
	}
	
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, (GLvoid *)offset);
	glutSwapBuffers();
	glutPostRedisplay();
}

static void on_close(void)
{
	// do cleanup
	cleanup();
}

static void on_key_press(int key, int x, int y)
{
	switch(key)
	{
		case '0': case '1': case '2': case '3': case '4':
			gl.show_mode = key - 0x30;
			break;
		case 27: // esc
			gl.show_mode = 0;
			break;
		case GLUT_KEY_UP: gl.show_mode = 1; break;
		case GLUT_KEY_DOWN: gl.show_mode = 2; break;
		case GLUT_KEY_LEFT: gl.show_mode = 3; break;
		case GLUT_KEY_RIGHT: gl.show_mode = 4; break;
		default:
			printf("key = %d\n", (int)key);
			break;
	}
}

static void on_timer(int id)
{
	if(0 != id)
	{
		char title[512];
		sprintf(title, "%s: %d Frames Per Second @ %d x %d", 
			app_name,
			gl.frame_count * 4,
			gl.width, gl.height);
		
		glutSetWindowTitle(title);			
	}
	gl.frame_count = 0;
	glutTimerFunc(250, on_timer, 1);
}
static void on_idle(void)
{
	glutPostRedisplay();
}


static void cleanup()
{
	shaders_destroy();
	vbo_destroy();
}


//*****************************
//** utilities
static void vbo_create()
{
	GLenum err_code = glGetError();
	Vertex_t Vertices[] =
	{
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		// Top
		{ { -0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		// Bottom
		{ { -0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.0f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		// Left
		{ { -0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		// Right
		{ { 0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }
	};
	
	GLubyte Indices[] = {
		// Top
		0, 1, 3,
		0, 3, 2,
		3, 1, 4,
		3, 4, 2,
		// Bottom
		0, 5, 7,
		0, 7, 6,
		7, 5, 8,
		7, 8, 6,
		// Left
		0, 9, 11,
		0, 11, 10,
		11, 9, 12,
		11, 12, 10,
		// Right
		0, 13, 15,
		0, 15, 14,
		15, 13, 16,
		15, 16, 14
	};
	
	const size_t buffer_size = sizeof(Vertices);
	const size_t vertex_size = sizeof(Vertices[0]);
	const size_t color_offset = sizeof(Vertices[0].xyzw);

	
	glGenVertexArrays(1, &gl.vao_id);
	glBindVertexArray(gl.vao_id);
	
	glGenBuffers(1, &gl.vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, gl.vao_id);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertex_size, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid *)color_offset);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glGenBuffers(1, &gl.index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl.index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	
	gl_check_error(err_code);
	
	
}
static void vbo_destroy()
{
	GLenum err_code = glGetError();
	
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &gl.vbo_id);	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &gl.index_buffer_id);	
	
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &gl.vao_id);
	
	
	gl_check_error(err_code);
}

