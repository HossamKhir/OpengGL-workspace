//============================================================================
// Name        : main.cpp
// Author      : Hossam Khair
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

int
main(void)
{
	if(!glfwInit())
	{
		cout << "Failed to initialise GLFW\n" << endl;
		return -1;
	}

	GLFWwindow * window = glfwCreateWindow(800, 600, "Window", 0, 0);
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		cout << "Failed to initialise GLAD\n" << endl;
		return -1;
	}

	while(!glfwWindowShouldClose(window))
	{
		// programme goes in here
	}

	glfwTerminate();

	return 0;
}
