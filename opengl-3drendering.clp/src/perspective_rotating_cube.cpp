/*
 * perspective_rotating_cube.cpp
 *
 *  Created on: 18 Mar 2020
 *      Author: hossam
 */




#include "perspective_rotating_cube.h"

using namespace std;
using namespace glm;

//============================================================================
// shader source codes
// vertex shader: transforms the geometry
const GLchar* pglcPerspectiveVertex120 = R"END(
	#version 120
	attribute vec3 position;
	attribute vec3 colour;
	attribute vec2 inUVs;

	varying vec3 outColour;
	varying vec2 outUVs;
	
	uniform float time;
	uniform mat4 matrix;
	uniform mat4 projection;	// for projection matrix
	
	void main()
	{
		float theta = time;
		
		float co = cos(theta);
		float si = sin(theta);
		
		mat4 rotationY = mat4(co, 0, si,  0,
							0,  1,  0,  0,
							-si,  0, co, 0,
							0,  0,  0,  1);

		co = cos(theta/2.);
		si = sin(theta/2.);

		mat4 rotationX = mat4(1, 0, 0, 0,
							0, co, -si, 0,
							0, si, co, 0,
							0, 0, 0, 1);

		outColour = colour;
		outUVs = inUVs;
		// multiply by projection matrix at first
		gl_Position = projection * matrix * rotationY * rotationX * vec4(position,1.f);
	}
	)END";
// fragment shader: fills the screen
const GLchar* pglcPerspectiveRaster120 = R"END(
	#version 120
	varying vec3 outColour;
	varying vec2 outUVs;

	uniform sampler2D texture; // 1st texture slot by default
	uniform float time;
	
	void main()
	{
		// half transparent texture
		gl_FragColor = vec4(texture2D(texture, outUVs)/2.f + vec4(outColour,1.f)/2.f);
	}
	)END";
//============================================================================


int
perspective_rotating_cube(void)
{
	GLFWwindow* glfwWindow;

	if (!glfwInit())
	{
		cout << "error!\n";
		return -1;
	}

	glfwWindow = glfwCreateWindow((0.9f * SCREEN_WIDTH), (0.9f * SCREEN_HEIGHT), "Modern OpenGL\n", 0, 0);

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
	glShaderSource(gluVertexShader, 1, &pglcPerspectiveVertex120, 0);

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
	glShaderSource(gluFragmentShader, 1, &pglcPerspectiveRaster120, 0);

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
	// indexed drawing
	/*
			indexed drawing: each vertex must have a unique index, and each triangle is
			defined by indices not vertices
			    6
	 *
			  /   \
		  5 *      * 2
			|\ 1  /|
			|  *   |
			|  |   |
		  4 *  |   * 3 <-- 7 is the last vertex behind the visible faces
			 \ |  /
	 *
			   0
	 */

	// setting up the cube using indexed drawing
	// defining the vertices in clockwise direction
	GLfloat glfVertices[] = {
			-1, -1, +1, // index 0
			-1, +1, +1,	// index 1
			+1, +1, +1,	// index 2
			+1, -1, +1,	// index 3
			-1, -1, -1,	// index 4
			-1, +1, -1,	// index 5
			+1, +1, -1, // index 6
			+1, -1, -1, // index 7
			/* in order to put a texture on a face,
			sperate vertices should be defined for such face to avoid artifacts */
			// the same vertices of the front face
			-1, -1, +1, // index 8
			-1, +1, +1,	// index 9
			+1, +1, +1,	// index 10
			+1, -1, +1,	// index 11
	};

	/* defining colours for vertices, it must be in the same order as the
		vertices as it will be indexed as well */
	GLfloat glfColours[] = {
			1, 0, 0, // red, green, blue
			0, 1, 0,
			0, 0, 1,
			1, 0, 1,
			1, 1, 0,
			0, 1, 1,
			0, 1, 0,
			1, 0, 0,
			/* what applies on vertices,
			applies on colours */
			// define white colour
			1, 1, 1,
			1, 1, 1,
			1, 1, 1,
			1, 1, 1
	};

	/* defining the indices, to refer to the vertices to create triangles using
		indices only */
	/* drawing wireframes is done by drawing edges instead of triangles */
	GLubyte glubIndices[] = {
			0, 1,	// front face, edge 0
			1, 2,	// front face, edge 1
			2, 3,	// front face, edge 2
			3, 0,	// front face, edge 3
			0, 4,	// left face, edge 0
			4, 5,	// left face, edge 1
			5, 1,	// left face, edge 2
			1, 0,	// left face, edge 3
			1, 5,	// top face, edge 0
			5, 6,	// top face, edge 1
			6, 2,	// top face, edge 2
			2, 1,	// top face, edge 3
			3, 2,	// right face, edge 0
			2, 6,	// right face, edge 1
			6, 7,	// right face, edge 2
			7, 3,	// right face, edge 3
			7, 6,
			6, 5,
			5, 4,
			4, 7,
			0, 3,
			3, 7,
			7, 4,
			4, 0
	};
	// adjacent edges will be overdrawn
	//============================================================================
	// defining VBOs
	// vertices buffer
	GLuint gluiVerticesBuffer;
	glGenBuffers(1, &gluiVerticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gluiVerticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glfVertices), glfVertices, GL_STATIC_DRAW);

	// colours buffer
	GLuint gluiColoursBuffer;
	glGenBuffers(1, &gluiColoursBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gluiColoursBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glfColours), glfColours, GL_STATIC_DRAW);

	//indices buffer
	GLuint gluiIndicesBuffer;
	glGenBuffers(1, &gluiIndicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gluiIndicesBuffer);	// indices are used in elements buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glubIndices), glubIndices, GL_STATIC_DRAW);
	//============================================================================
	// defining attributes
	GLuint gluiAttribPosition;
	gluiAttribPosition = glGetAttribLocation(gluShaderProgramme, "position");
	glEnableVertexAttribArray(gluiAttribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, gluiVerticesBuffer);
	glVertexAttribPointer(gluiAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint gluiAttribColour;
	gluiAttribColour = glGetAttribLocation(gluShaderProgramme, "colour");
	glEnableVertexAttribArray(gluiAttribColour);
	glBindBuffer(GL_ARRAY_BUFFER, gluiColoursBuffer);
	glVertexAttribPointer(gluiAttribColour, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//============================================================================
	GLfloat glfMatrix[] = {
			0.5, 0, 0, 0,
			0, 0.5, 0, 0,
			0, 0, 0.5, 0,
			0, 0, 0, 1
	};

	GLuint gluiAttribMatrix;
	gluiAttribMatrix = glGetUniformLocation(gluShaderProgramme, "matrix");
	glUniformMatrix4fv(gluiAttribMatrix, 1, GL_FALSE, glfMatrix);

	GLuint gluiUniformTime;
	gluiUniformTime = glGetUniformLocation(gluShaderProgramme, "time");
	//============================================================================
	// texture attributes: UVs
	GLfloat glfUVs[] = {
			0, 0,	// the zero is for vertices that should not be textured
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,	// full rectangle for additional vertices
			0, 1,
			1, 1,
			1, 0,
	};

	GLuint gluiUVData;
	glGenBuffers(1, &gluiUVData);
	glBindBuffer(GL_ARRAY_BUFFER, gluiUVData);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glfUVs), glfUVs, GL_STATIC_DRAW);

	GLuint gluiAttribUVs;
	gluiAttribUVs = glGetAttribLocation(gluShaderProgramme, "inUVs");
	glEnableVertexAttribArray(gluiAttribUVs);
	glBindBuffer(GL_ARRAY_BUFFER, gluiUVData);
	glVertexAttribPointer(gluiAttribUVs, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//============================================================================
	// fixing the order of drawing problem
	// disabling backface culling for wireframe
	// glEnable(GL_CULL_FACE);	// clockwise backface culling
	//============================================================================
	// selecting line width
	glLineWidth(5);
	//============================================================================
	// to move the cube further, a scale matrix is required
	mat4 m4ScaleMatrix = mat4(1.0f);	// instantiates an identity matrix
	// move the matrix 2 units inside the screen
	m4ScaleMatrix = translate(m4ScaleMatrix, vec3(0, 0, -2));
	//============================================================================
	// projection matrix
	mat4 m4ProjectionMatrix	= glm::perspective(glm::radians(60.0f),
			(float)(SCREEN_WIDTH/SCREEN_HEIGHT),
			0.0f,
			10.0f) * m4ScaleMatrix;

	GLint gliUniformProjection = glGetUniformLocation(gluShaderProgramme, "projection");
	glUniformMatrix4fv(gliUniformProjection, 1, GL_FALSE, value_ptr(m4ProjectionMatrix));
	//============================================================================
	// render loop
	while (!glfwWindowShouldClose(glfwWindow))
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = glfwGetTime();
		glUniform1f(gluiUniformTime, time);

		//============================================================================
		// indexed drawing requires call of a different function
		// drawing wireframes require line element
		glDrawElements(GL_LINES, sizeof(glubIndices), GL_UNSIGNED_BYTE, 0);
		//============================================================================
		// glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
