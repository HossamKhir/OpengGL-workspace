#include "glfw-basic-app.h"

using namespace std;

int
glfw_basic_app(void)
{
	GLFWwindow * window;

	if (!glfwInit()) {
		cout << "Init error";
		return -1;
	}

	window = glfwCreateWindow(800, 600, "Basic App", 0, 0);	/* everything that is rendered 
															should be presented on a window */
	if (!window) {
		cout << "Window creation error";
		glfwTerminate();	// the application termination before exit
		return -1;
	}

	// the context contains the state of the openGL app
	glfwMakeContextCurrent(window);	// associating the 'window' context with the current one
	
	bool toggle = !true;

	// to allow for handling events
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// condition to toggle colour on screen
		if (toggle)
		{
			glClearColor(0.0, 0.5, 0.0, 0);	// specifies a colour 
		}
		else
		{
			glClearColor(0.0, 0.0, 0.5, 0);	// specifies a colour 
		}
		
		toggle = !toggle;	// changing the status to toggle next frame
		
		glClear(GL_COLOR_BUFFER_BIT);	// clearing the screen buffer
		// GL_COLOR_BUFFER_BIT is a flag

		glfwSwapBuffers(window);	// represent the buffering onto an actual window
		/* openGL renders on off screen buffer; the glfwSwapBuffers is what allows for
		presenting just that */
		glfwPollEvents();	// processes current events
	}

	glfwTerminate();	// each application must be terminated before programme's end
}