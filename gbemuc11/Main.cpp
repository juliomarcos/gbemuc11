#include <Windows.h>

#include <GLFW\glfw3.h>
#include <iostream>
#include <string>

#include "RomPath.hpp"

using namespace std;

namespace gbemu {

	GLFWwindow* initWindow(int width, int height, string windowTitle) {
		GLFWwindow* window;

		/* Initialize the library */
		if (!glfwInit()) {
			throw runtime_error("Can't initialize glfw");
		}

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			throw runtime_error("Can't create a window");
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		return window;
	}

}

int main(int argc, char *argv[], char* envp[]) {

	auto romPath = gbemu::getRomPathFromArgvOrFail(argc, argv);
	if (romPath.empty()) {
		MessageBox(0, L"You need to specify a gameboy rom.", L"Loading Error", MB_ICONERROR);
		return -1;
	}

	auto romFileName = gbemu::getRomFileNameFromPath(romPath);
	auto windowTitle = string("gbemuc11 - ") + romFileName;

	auto window = gbemu::initWindow(640, 480, windowTitle);

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}