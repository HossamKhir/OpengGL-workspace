#include <iostream>
#include <math.h>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

using namespace std;

int
main(void)
{
	if (!glfwInit())
	{
		cout << "Failure to initialise GLFW\n" << endl;
		return -1;
	}

	GLFWwindow * window = glfwCreateWindow(800, 600, "Main\n", 0, 0);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failure to load OpenGL\n" << endl;
		return -1;
	}


	while (!glfwWindowShouldClose(window))
	{
		// programme goes in here
	}

	cout << "Hello World!" << endl;

	char c = 0;

	cin >> c;

	return 0;
}