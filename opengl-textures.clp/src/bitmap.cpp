#include "bitmap.h"

using namespace std;

//============================================================================
// shader source codes
// vertex shader: transforms the geometry
const GLchar* pglcBitmapVertex120 = R"END(
	 #version 120
	 attribute vec3 inPosition;
	 //============================================================================
	 // for texture
	 attribute vec2 inUVs;
	 varying vec2 outUVs;
	 //============================================================================
	 // for reversed texture
	 uniform mat4 matrix;
	 //============================================================================
	 void main()
	 {
		 outUVs = inUVs;
		 gl_Position = matrix * vec4(inPosition, 1);
		 // multiplication is actually translation
	 }
	 )END";
// fragment shader: fills the screen
const GLchar* pglcBitmapRaster120 = R"END(
	 #version 120
	 uniform vec2 resolution;	// required for proper gradient transition
	 // define a time variable
	 uniform float time;
	 //============================================================================
	 // for texture
	 varying vec2 outUVs;
	 uniform sampler2D texture;	// to access texture data, default 1st texture slot
	 //============================================================================
	 void main()
	 {
		 //============================================================================
		 // sampling the image
		 gl_FragColor = texture2D(texture, outUVs);
		 //============================================================================
	 }
	 )END";
//============================================================================

int
bitmap(void)
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
	glShaderSource(gluVertexShader, 1, &pglcBitmapVertex120, 0);

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
	glShaderSource(gluFragmentShader, 1, &pglcBitmapRaster120, 0);

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
	// the texture image is printed reversed, one solution is transformation matrix
	GLfloat glfMatrix[] = {	// an identity matrix
		-1, 0, 0, 0,		// reversing x axis
		0, -1, 0, 0,		// reversing y axis, both allow for rotation around z axis
		0, 0, 1, 0,
		0, 0, 0, 1
	};
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
	// define texture coordinates
	GLfloat glfUVs[] = {
		0, 0,	// this is at the bottom left
		0, 1,	// this is at top left
		1, 0,	// this is at bottom right
		1, 0,
		0, 1,
		1, 1	// this is at top right
	}; 
	//============================================================================
	// generate VBO for UV data
	GLuint gluiUVsData;
	glGenBuffers(1, &gluiUVsData);
	glBindBuffer(GL_ARRAY_BUFFER, gluiUVsData);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glfUVs), glfUVs, GL_STATIC_DRAW);
	//============================================================================
	// bitmap
	bmpread_t bitmap;

	if(!bmpread("texture.bmp", 0, &bitmap))	// to read the bmp file
	{
		cout << "Texture loading error\n" << endl;
		exit(-1);
	}
	//============================================================================
	// generating texture resource
	GLuint gluiTextureID;
	glGenTextures(1, &gluiTextureID);
	glBindTexture(GL_TEXTURE_2D, gluiTextureID);
	//============================================================================
	// texture parameters for filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//============================================================================
	// provide the texture data that matches the bitmap
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.data);

	//============================================================================
	GLuint gluiAttribMatrix;
	// find the uniform location, as usual
	gluiAttribMatrix = glGetUniformLocation(gluShaderProgramme, "matrix");
	glUniformMatrix4fv(gluiAttribMatrix, 1/*it is one unit of matrix*/, GL_FALSE, glfMatrix);


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
	// setting up attributes for UV data
	GLuint gluiAttribUVs;
	gluiAttribUVs = glGetAttribLocation(gluShaderProgramme, "inUVs");
	glEnableVertexAttribArray(gluiAttribUVs);
	glBindBuffer(GL_ARRAY_BUFFER, gluiUVsData);
	// setup vertix attribute pointer
	glVertexAttribPointer(gluiAttribUVs,
							2/*the attribute consist of 2 floats*/,
							GL_FLOAT,
							GL_FALSE, /*normalise the attribute*/
							0,
							0);

	//============================================================================
	// resolution uniform
	GLuint gluUniformResolution;
	gluUniformResolution = glGetUniformLocation(gluShaderProgramme,"resolution");
	glUniform2f(gluUniformResolution,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT);
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