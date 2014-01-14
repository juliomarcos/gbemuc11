#include <Windows.h>

#include <GLFW\glfw3.h>
#include "StdLibraries.hpp"

#include "RomPath.hpp"
#include "CPU.hpp"

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

	shared_ptr<array<byte, CARTRIDGE_SIZE>> getByteBufferFromPath(string path) {
		ifstream romFile;
		romFile.open(path, ios::binary);
		auto bufferPtr = make_shared<array<byte, CARTRIDGE_SIZE>>();
		romFile.read(reinterpret_cast<char*>((*bufferPtr).data()), CARTRIDGE_SIZE);
		return bufferPtr;
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
	auto window = gbemu::initWindow(640, 576, windowTitle);

	auto cpu = gbemu::CPU::CPU();

	for (size_t i = 0; i < 1000; i++)
	{
		cpu.loadROM(gbemu::getByteBufferFromPath(romPath));
	}


	while (!glfwWindowShouldClose(window))
	{
		cpu.emulateCycle();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}