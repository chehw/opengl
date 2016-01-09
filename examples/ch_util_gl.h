#ifndef _CH_UTIL_GL_H_
#define _CH_UTIL_GL_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>


#define GLUT_CONTEXT_MAJOR_VERSION 3
#define GLUT_CONTEXT_MINOR_VERSION 3

#define gl_err_exit(err_code) \
	do{ \
		fprintf(stderr, "[%s] - [%s] @ (line %d): \n(err_code = %d): %s\n", \
				__FILE__, __func__, __LINE__, \
				err_code, gluErrorString(err_code)); \
		exit(1); \
	}while(0)

#define gl_check_error(err_code) \
	do{ \
		err_code = glGetError(); \
		if(err_code != GL_NO_ERROR) gl_err_exit(err_code); \
	}while(0)
	
#ifdef _DEBUG_INFO
	#define info_log2(fd, fmt, ...) \
		do{ fprintf(fd, "[INFO]: [%s] - [%s] @ (line %d)\n\t", __FILE__, __func__, __LINE__); \
			fprintf(fd, fmt, ##__VA_ARGS__); fprintf(fd, "\n");} \
		while(0)
	#define info(fmt, ...)  \
		do{ fprintf(stdout, "[INFO]: [%s] - [%s] @ (line %d)\n\t", __FILE__, __func__, __LINE__); \
			fprintf(stdout, fmt, ##__VA_ARGS__ ); fprintf(stdout, "\n");} \
		while(0)
#else
	#define info_log2(fd, fmt, ...) do{ } while(0)
	#define info(fmt, ...) do{ }while(0)
#endif

#if defined(__cplusplus)
extern "C" {
#endif



#if defined(__cplusplus)
}
#endif
#endif
