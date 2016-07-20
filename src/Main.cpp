#include <GLFW/glfw3.h>
#include <execinfo.h>
#include "StdLibraries.hpp"
#include "TypeDefinitions.hpp"

#include "RomPath.hpp"
#include "CPU.hpp"
#include "GPU.hpp"

using namespace std;

namespace gbemu {
	
	const int REFRESH_RATE = 60; // Hz

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
		glfwSwapInterval(1);

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

void handler() {
    void *trace_elems[20];
    int trace_elem_count(backtrace( trace_elems, 20 ));
    char **stack_syms(backtrace_symbols( trace_elems, trace_elem_count ));
    for (int i = 0 ; i < trace_elem_count ; ++i)
    {
        std::cout << stack_syms[i] << "\n";
    }
    free(stack_syms);

    exit(1);
}   


int main(int argc, char *argv[]) {

	set_terminate(handler);

	auto romPath = gbemu::getRomPathFromArgvOrFail(argc, argv);
	if (romPath.empty()) {
		cout << "You need to specify a gameboy rom. e.g >> gbemu Tetris.gb";
		return -1;
	}

	auto romFileName = gbemu::getRomFileNameFromPath(romPath);
	auto windowTitle = string("gbemuc11 - ") + romFileName;
	auto window = gbemu::initWindow(160, 144, windowTitle);

	auto cpu = gbemu::CPU::CPU();
	auto interrupt = gbemu::Interrupt(cpu);
	auto gpu = gbemu::GPU::GPU(window, cpu, interrupt);

	//cpu.loadRom(gbemu::getByteBufferFromPath(romPath)); // TODO: usar isto depois q o bootstrap rodar
	cpu.loadRom(gbemu::getByteBufferFromPath("./build/bootstrap.bin"));

	auto cycles = 1;
	//auto cycles = 3 + 8191*3 + 9;
	// for(size_t i = 0; i < cycles; ++i)
	// {
	// cpu.emulateCycle();
	// }

    const auto CYCLES_PER_FRAME = gbemu::CLOCK_SPEED / gbemu::REFRESH_RATE;
	while (!glfwWindowShouldClose(window))
	{
		auto cyclesThisFrame = 0;
		auto cyclesThisInstruction = 0;
		
		while (cyclesThisFrame < CYCLES_PER_FRAME) {			
			cyclesThisInstruction = cpu.emulateNextInstruction();
			gpu.draw(cyclesThisInstruction);
			//timers.update(cyclesThisInstruction);
			interrupt.run();
			cyclesThisFrame += cyclesThisInstruction;
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		// TOOD: ETST
		cycles -= cyclesThisInstruction;
		if (cycles <= 0) break;
	}

	glfwTerminate();
	return 0;
}