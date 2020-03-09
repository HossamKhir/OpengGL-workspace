/*
 * circle.cpp
 *
 *  Created on: 9 Mar 2020
 *      Author: hossam
 */




#include "circle.h"

using namespace std;

//============================================================================
// shader source codes
// vertex shader: transforms the geometry
const GLchar* pglcCircleVertex120 = R"END(
	 #version 120
	 attribute vec3 inPosition;
	 void main()
	 {
		 gl_Position = vec4(inPosition, 1);
	 }
	 )END";
// fragment shader: fills the screen
const GLchar* pglcCircleRaster120 = R"END(
	 #version 120
	 uniform vec2 resolution;	// required for proper gradient transition
	 // define a time variable
	 uniform float time;
	 void main()
	 {
		 //============================================================================
		 // drawing a circle
		 vec2 centre = resolution / 2.0f;	// define centre point
		 
		 vec2 currentPoint = gl_FragCoord.xy;	// get current point
		 // gl_FragCoord is vec4, hence we need xy only for vec2

		 // check location of current point, inside or outside the circle
		 if(length(currentPoint - centre) < 100.0f)	// 100.0f is supposedly the radius
		 {
			 gl_FragColor = vec4(1, 1, 1, 1);	// white
		 }
		 else
		 {
			 gl_FragColor = vec4(0, 0, 0, 1);	// black
		 }
		 //============================================================================
	 }
	 )END";
//============================================================================

int
circle(void)
{
	GLFWwindow* glfwWindow;

	if (!glfwInit())
	{
		cout << "error!\n";
		return -1;
	}

	glfwWindow = glfwCreateWindow((0.9f * SCREEN_WIDTH), (0.9f * SCREEN_HIEGHT), "Modern OpenGL\n", 0, 0);

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
	//============================================================================
	// instantiating vertex shader object
	GLuint gluVertexShader = glCreateShader(GL_VERTEX_SHADER);

	// assign source code for shader
	glShaderSource(gluVertexShader, 1, &pglcCircleVertex120, 0);

	// compile the shader
	glCompileShader(gluVertexShader);
	//============================================================================
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
	//============================================================================
	// instantiating fragment shader object
	GLuint gluFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code for shader
	glShaderSource(gluFragmentShader, 1, &pglcCircleRaster120, 0);

	// compile shader's source code
	glCompileShader(gluFragmentShader);
	//============================================================================
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
	//============================================================================
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
	//============================================================================
	// Multiple VBOs
	// there can be a set of input buffers, each representing a parameter (colour, coordinates)
	// these buffers are then must be bound to the vertex shader

	// vertex coordinates
	// using 2 different VBOs
	GLfloat glfPositions[] = {	// selecting clockwise
			-1,	-1,	0,
			-1,	 1,	0,
			1,	-1,	0,
			1,	-1,	0,
			-1,	 1,	0,
			1,	 1,	0
	};

	// creating vertex buffer objects
	// VBO handle
	GLuint gluDataPositions;
	// generate buffer
	glGenBuffers(1, &gluDataPositions);	// gluDataPositions is the handle for the buffer now
	// bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, gluDataPositions);	// acts as a gateway
	// send the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(glfPositions), glfPositions, GL_STATIC_DRAW); // acts like memcpy() but from RAM to GPU
	//============================================================================
	// setting up attributes
	GLuint gluAttribPosition;

	// get the location of the attribute
	gluAttribPosition = glGetAttribLocation(gluShaderProgramme, "inPosition");
	// enable the attribute
	glEnableVertexAttribArray(gluAttribPosition);	// some shader can dynamically disable/enable attributes
	// bind the buffer to link the data from position VBO to attribute
	glBindBuffer(GL_ARRAY_BUFFER, gluDataPositions);
	// configure attribute stripe, specify the format
	glVertexAttribPointer(gluAttribPosition, 3, GL_FLOAT, GL_FALSE, 0 /* stripe size is currently irrelevant */, 0);

	//============================================================================
	// resolution uniform
	GLuint gluUniformResolution;
	gluUniformResolution = glGetUniformLocation(gluShaderProgramme,"resolution");
	glUniform2f(gluUniformResolution,(float)SCREEN_WIDTH,(float)SCREEN_HIEGHT);
	//============================================================================
	// uniform time
	GLuint gluUniformTime;
	gluUniformTime = glGetUniformLocation(gluShaderProgramme, "time");
	//============================================================================
	// render loop
	while (!glfwWindowShouldClose(glfwWindow))
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		//============================================================================
		// each update loop
		float fltTime = glfwGetTime();
		glUniform1f(gluUniformTime, fltTime);
		//============================================================================

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
