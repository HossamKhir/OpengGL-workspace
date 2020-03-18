/*
 * rotating_cube.cpp
 *
 *  Created on: 18 Mar 2020
 *      Author: hossam
 */




#include "rotating_cube.h"

using namespace std;

//============================================================================
// shader source codes
// vertex shader: transforms the geometry
const GLchar* pglcRotatingCubeVertex120 = R"END(
	#version 120
	attribute vec3 position;
	attribute vec3 colour;
	attribute vec2 inUVs;

	varying vec3 outColour;
	varying vec2 outUVs;
	
	uniform float time;
	uniform mat4 matrix;
	uniform mat4 projection;
	
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
		gl_Position = matrix * rotationY * rotationX * vec4(position,1.f);
	}
	)END";
// fragment shader: fills the screen
const GLchar* pglcRotatingCubeRaster120 = R"END(
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
rotating_cube(void)
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
	glShaderSource(gluVertexShader, 1, &pglcRotatingCubeVertex120, 0);

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
	glShaderSource(gluFragmentShader, 1, &pglcRotatingCubeRaster120, 0);

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
		separate vertices should be defined for such face to avoid artifacts */
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
	GLubyte glubIndices[] = {
		0, 1, 2,	// front right face, triangle 0
		0, 2, 3,	// front right face, triangle 1
		0, 4, 5,	// front left face, triangle 0
        0, 5, 1,	// front left face, triangle 1
        1, 5, 6,	// top face, triangle 0
        1, 6, 2,	// top face, triangle 1
        3, 2, 6,	// back right face, triangle 0
        3, 6, 7,	// back right face, triangle 1
        4, 0, 3,	// bottom face, triangle 0
		4, 3, 7,	// bottom face, triangle 1
        7, 6, 5,	// back left face, triangle 0
        7, 5, 4,	// back left face, triangle 1
		/* now the new indices */
		8, 9, 10,
		8, 10, 11,
	};
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
	// texture
	/* the bmpread library is very sensitive to bmp format;
	so the bmp must be 32 bit in depth */
	bmpread_t bitmap;

	if(!bmpread("texture.bmp", 0, &bitmap))
	{
		cout << "texture loading error" << endl;
		exit(-1);
	}

	GLuint gluiTextureID;
	// generating texture
    glGenTextures(1, &gluiTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gluiTextureID);

	// setting texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D,0,3,bitmap.width,bitmap.height,0,GL_RGB,GL_UNSIGNED_BYTE,bitmap.data);

    GLuint attribTex = glGetAttribLocation(gluShaderProgramme, "texture");
    glUniform1i(attribTex, 0);
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
	glEnable(GL_CULL_FACE);	// clockwise backface culling
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
		glDrawElements(GL_TRIANGLES, sizeof(glubIndices), GL_UNSIGNED_BYTE, 0);
		//============================================================================
		// glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
