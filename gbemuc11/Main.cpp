#include <GLFW\glfw3.h>
#include <iostream>
#include <string>

using namespace std;

string getRomPathFromArgvOrFail(int argc, char* argv[]) {
	if (argc < 2) {
		throw invalid_argument("you need to specify a gameboy ROM");
	}
	return string(argv[1]);
}

int main(int argc, char *argv[], char* envp[]) {

	auto romPath = getRomPathFromArgvOrFail(argc, argv);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}