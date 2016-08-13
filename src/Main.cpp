#include "imgui_impl_glfw.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <execinfo.h>
#include "StdLibraries.hpp"
#include "TypeDefinitions.hpp"

#include "RomPath.hpp"
#include "CPU.hpp"
#include "GPU.hpp"
#include "Logger.hpp"
#include "Debugger.hpp"

gbemu::LogLevel gbemu::Log::currentLogLevel = gbemu::LogLevel::ERROR;
const int usingDebugger = true;

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

		//glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		return window;
	}

	vector<char> getByteBufferFromPath(string path) {
		ifstream file(path.c_str(), ios::in|ios::binary|ios::ate);
		vector<char> buffer;
		int length = file.tellg();
		if (length) {
			file.seekg(0, ios::beg);
			buffer.resize((size_t)length);
			file.read(&buffer.front(), (size_t)length);	
		}
		return buffer;
	}

}

void handler() {
    void *trace_elems[20];
    int trace_elem_count(backtrace(trace_elems, 20));
    char **stack_syms(backtrace_symbols( trace_elems, trace_elem_count ));
    for (int i = 0 ; i < trace_elem_count ; ++i) {
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
	
	// Setup ImGui binding
	GLFWwindow* debuggerWindow;
	ImVec4 clearColor = ImColor(114, 144, 154);
	if (usingDebugger) {
		debuggerWindow = gbemu::initWindow(800, 830, "debugger");
		glfwMakeContextCurrent(debuggerWindow);
	    ImGui_ImplGlfw_Init(debuggerWindow, true);
		int debuggerX, debuggerY;
		glfwGetWindowPos(debuggerWindow, &debuggerX, &debuggerY);
		glfwSetWindowPos(window, debuggerX+800, debuggerY);	
	}

	auto cpu = gbemu::CPU::CPU();
	auto interrupt = gbemu::Interrupt(cpu);
	auto gpu = gbemu::GPU::GPU(window, cpu, interrupt);
	gpu.initGraphics();

	cpu.setRomBuffer(gbemu::getByteBufferFromPath(romPath));
	cpu.loadRom(cpu.getRomBuffer(), 0x8000); // TODO: usar isto depois q o bootstrap rodar
	cpu.loadRom(gbemu::getByteBufferFromPath("./build/bootstrap.bin"), 0x100);

    const auto CYCLES_PER_FRAME = gbemu::CLOCK_SPEED / gbemu::REFRESH_RATE;
	while (!glfwWindowShouldClose(window))
	{
		auto cyclesThisFrame = 0;
		auto cyclesThisInstruction = 0;
		
		// TODO: do not define this inside this loop
		auto cycle = [&]() {
			cyclesThisInstruction = cpu.emulateNextInstruction();
			gpu.draw(cyclesThisInstruction);
			//timers.update(cyclesThisInstruction);
			interrupt.run();
			cyclesThisFrame += cyclesThisInstruction;
		};
		
		while (cyclesThisFrame < CYCLES_PER_FRAME) {
			if (usingDebugger) {
				if (!debugger::seekingPc) {
					glfwMakeContextCurrent(debuggerWindow);
					glfwPollEvents();
					ImGui_ImplGlfw_NewFrame();
					if (debugger::ShouldExecuteNextInstruction(cpu)) {
						cycle();
					}
					
					debugger::MemoryViewer(cpu);
					debugger::RegistersViewer(cpu);	
			        
			        int displayW, displayH;
			        glfwGetFramebufferSize(debuggerWindow, &displayW, &displayH);
			        glViewport(0, 0, displayW, displayH);
			        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
			        glClear(GL_COLOR_BUFFER_BIT);
			        ImGui::Render();
			        glfwSwapBuffers(debuggerWindow);
				} else {
					cycle();
					if (cpu._pc == debugger::pcTarget) {
						debugger::seekingPc = false;
					}
				}
			} else {
				cycle();
			}
		}
		
		glfwMakeContextCurrent(window);
		gpu.drawPixels();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();
	return 0;
}