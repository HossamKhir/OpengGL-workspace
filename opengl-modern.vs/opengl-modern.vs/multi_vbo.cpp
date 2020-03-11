#include "multi_vbo.h"

using namespace std;

// shader source codes
// vertex shader: transforms the geometry
const GLchar* pglcMultiVBOVertex120 = R"END(
	 #version 120
	 attribute vec3 inColour;
	 attribute vec3 inPosition;
	 uniform mat4 matrix;
	 varying vec3 outColour;
	 void main()
	 {
		outColour = inColour;
		gl_Position = vec4(inPosition, 1);
	 }
	 )END";
// fragment shader: fills the screen
const GLchar* pglcMultiVBORaster120 = R"END(
	 #version 120
	 varying vec3 outColour;
	 void main()
	 {
		gl_FragColor = vec4(outColour,1);
	 }
	 )END";
//============================================================================

int
multi_vbo(void)
{
	GLFWwindow* glfwWindow;

	if (!glfwInit())
	{
		cout << "error!\n";
		return -1;
	}

	glfwWindow = glfwCreateWindow((0.9f * SCREEN_WIDTH), (0.9 * SCREEN_HEIGHT), "Modern OpenGL\n", 0, 0);

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
	glShaderSource(gluVertexShader, 1, &pglcMultiVBOVertex120, 0);

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
	glShaderSource(gluFragmentShader, 1, &pglcMultiVBORaster120, 0);

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
	GLfloat positions[] = {	// selecting clockwise
		-1,	-1,	0,
		-1,	 1,	0,
		 1,	-1,	0,
		 1,	-1,	0,
		-1,	 1,	0,
		 1,	 1,	0
	};
	// colours
	GLfloat colours[] = {
		1,	0,	0,
		0,	1,	0,
		0,	0,	1,
		1,	1,	0,
		1,	0,	1,
		0,	1,	1
	};

	// creating vertex buffer objects
	// VBO handle
	GLuint dataPositions;
	// generate buffer
	glGenBuffers(1, &dataPositions);	// dataPositions is the handle for the buffer now
	// bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, dataPositions);	// acts as a getway
	// send the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); // acts like memcpy() but from RAM to GPU

	// repeat for the colours
	GLuint dataColours;
	glGenBuffers(1, &dataColours);
	glBindBuffer(GL_ARRAY_BUFFER, dataColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	//============================================================================
	// setting up attributes
	GLuint attribPosition;
	GLuint attribColour;

	// get the location of the attribute
	attribPosition = glGetAttribLocation(gluShaderProgramme, "inPosition");
	// enable the attribute
	glEnableVertexAttribArray(attribPosition);	// some shader can dynamically disable/enable attributes
	// bind the buffer to link the data from position VBO to attribute
	glBindBuffer(GL_ARRAY_BUFFER, dataPositions);
	// configure attribute stripe, specify the format
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0 /* stripe size is currently irrelevant */, 0);

	// repeat for colour
	attribColour = glGetAttribLocation(gluShaderProgramme, "inColour");
	glEnableVertexAttribArray(attribColour);
	glBindBuffer(GL_ARRAY_BUFFER, dataColours);
	glVertexAttribPointer(attribColour, 3, GL_FLOAT, GL_FALSE, 0, 0 /* there are no offsets in the stream of data */);
	//============================================================================
	// render loop
	while (!glfwWindowShouldClose(glfwWindow))
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}