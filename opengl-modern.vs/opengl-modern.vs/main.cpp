// modern_opengl.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "rotating_square.h"

using namespace std;

#include "rotating_square.h"

using namespace std;

const GLfloat aglfVertices[] = {
	-1.0f,	-1.0f,	0.0f,		// x, y, z
	1.0f,	-1.0f,	0.0f,
	1.0f,	1.0f,	0.0f,
	-1.0f,	-1.0f,	0.0f,
	1.0f,	1.0f,	0.0f,
	-1.0f,	1.0f,	0.0f
};

const GLfloat aglfColours[] = {
	0.0f, 0.0f, 1.0f,		// r, g, b
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};


// shader source codes
// vertex shader
const GLchar* pglcVertex120 = R"END(
	 #version 120
	 attribute vec4 inColour;
	 attribute vec4 inPosition;
	 uniform mat4 matrix;
	 varying vec4 outColour;
	 void main()
	 {
		outColour = inColour;
		gl_Position = inPosition * matrix;
	 }
	 )END";
// fragment shader
const GLchar* pglcRaster120 = R"END(
	 #version 120
	 varying vec4 outColour;
	 void main()
	 {
		gl_FragColor = outColour;
	 }
	 )END";
//=========================================================


int
main(void)
{
	GLFWwindow * glfwWindow;

	if (!glfwInit())
	{
		cout << "error!\n";
		return -1;
	}

	glfwWindow = glfwCreateWindow((0.9f * SCREEN_WIDTH), (0.9 * SCREEN_HIEGHT), "Modern OpenGL\n", 0, 0);

	if (!glfwWindow)
	{
		printf("Window creation error!\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(glfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failure to load OpenGL\n" << endl;
		return -1;
	}

	// instantiating vertex shader object
	GLuint gluVertexShader = glCreateShader(GL_VERTEX_SHADER);

	// assign source code for shader
	glShaderSource(gluVertexShader, 1, &pglcVertex120, 0);

	// compile the shader
	glCompileShader(gluVertexShader);
	//=========================================================
	// check status of compilation
	GLint pgliCompilationStatus;

	// get information about a shader: glGetShaderiv
	glGetShaderiv(gluVertexShader, GL_COMPILE_STATUS, &pgliCompilationStatus);

	// status of compilation
	if (pgliCompilationStatus == GL_FALSE)
	{
		// create a buffer to hold error message
		GLchar glcMessage[256];

		// get the error message
		glGetShaderInfoLog(gluVertexShader, sizeof(glcMessage), 0, glcMessage);

		// print out the error message
		cout << glcMessage << endl;

		// terminate programme
		exit(1);
	}
	//=========================================================================
	// instantiating fragment shader object
	GLuint gluFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code for shader
	glShaderSource(gluFragmentShader, 1, &pglcRaster120, 0);

	// compile shader's source code
	glCompileShader(gluFragmentShader);
	//=============================================================
	// get compile status
	glGetShaderiv(gluFragmentShader, GL_COMPILE_STATUS, &pgliCompilationStatus);

	// status of compilation
	if (pgliCompilationStatus == GL_FALSE)
	{
		// create a buffer to hold error message
		GLchar glcMessage[256];

		// get the error message
		glGetShaderInfoLog(gluFragmentShader, sizeof(glcMessage), 0, glcMessage);

		// print out the error message
		cout << glcMessage << endl;

		// terminate programme
		exit(1);
	}
	//============================================================================
	// creating new shader programme
	GLuint gluShaderProgramme = glCreateProgram();

	// attaching shaders to the programme
	glAttachShader(gluShaderProgramme, gluVertexShader);
	glAttachShader(gluShaderProgramme, gluFragmentShader);

	// linking shaders
	glLinkProgram(gluShaderProgramme);
	//====================================================
	// check programme linkage status
	glGetProgramiv(gluShaderProgramme, GL_LINK_STATUS, &pgliCompilationStatus);

	// linkage status
	if (pgliCompilationStatus == GL_FALSE)
	{
		// create a buffer to hold error message
		GLchar glcMessage[256];

		// get the error message
		glGetProgramInfoLog(gluShaderProgramme, sizeof(glcMessage), 0, glcMessage);

		// print out the error message
		cout << glcMessage << endl;

		// terminate programme
		exit(1);
	}

	// activate the programme
	glUseProgram(gluShaderProgramme);
	// unless changed, this means that all draw calls used afterwards use this context
	//=============================================================================
	// VBOs

	//=====
	// render loop
	while (!glfwWindowShouldClose(glfwWindow))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// set background colour to black
		glClear(GL_COLOR_BUFFER_BIT);

		// rotations are calculated by sine & cosine functions
		float fSinAlpha = 0.5 * sin(fAlpha);	// 0.5 if to zoom out the view
		float fCosAlpha = 0.5 * cos(fAlpha);	// 0.5 is like a scaling factor

		fAlpha += 0.1f;

		// defining rotation matrix
		const GLfloat aglfMatrix[] = {
			fSinAlpha,	-fCosAlpha,	0,	0,
			fCosAlpha,	fSinAlpha,	0,	0,
			0,			0,			1,	0,
			0,			0,			0,	1
		};

		// pass data to matrix attribute
		glUniformMatrix4fv(gluAttributeMatrix, 1, GL_FALSE, aglfMatrix);

		// draw the shape
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// GL_TRIANGLES: geometry primitive type
		// 6: number of indices

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}



