#include <iostream>
#include <GLFW/glfw3.h>
#include <math.h>

#include "triangle.h"
#include "glfw-basic-app.h"
#include "quad.h"
#include "circle.h"
#include "stack_of_matrices.h"

using namespace std;

int
main(void)
{
	GLFWwindow * window;

	if (!glfwInit()) {
		cout << "Init error";
		return -1;
	}

	window = glfwCreateWindow(800, 800, "Main", 0, 0);	/* everything that is rendered
														should be presented on a window */
	if (!window) {
		cout << "Window creation error";
		glfwTerminate();	// the application termination before exit
		return -1;
	}

	// the context contains the state of the openGL app
	glfwMakeContextCurrent(window);	// associating the 'window' context with the current one

	// setting up the matrix pipeline 
	glMatrixMode(GL_MODELVIEW);

	// load identity matrix
	glLoadIdentity();

	// some operations
	glScalef(0.1, 0.1, 1.0f);	// decreasing origin unit scale by 10; enhancing precision

	// to allow for handling events
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0, 0.0, 0.0, 0);	// specifies a colour (black)

		glClear(GL_COLOR_BUFFER_BIT);	// clearing the screen buffer
		
		vDrawSimpleSolarSystem();

		// vDrawTriangle(triRGB);

		// vDrawQuad(quaRGB);

		// represent the buffering onto an actual window
		glfwSwapBuffers(window);	/* openGL renders on off screen buffer; the glfwSwapBuffers is what allows for
									presenting just that */
		// processes current events
		glfwPollEvents();
	}

	// glfw_basic_app();

	return 0;
}
