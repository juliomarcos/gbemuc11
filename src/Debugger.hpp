#include "CPU.hpp"

namespace debugger {

	extern bool seekingPc;
	extern int pcTarget;

	bool ShouldExecuteNextInstruction(gbemu::CPU &cpu);

	void RegistersViewer(gbemu::CPU &cpu);

	void MemoryViewer(gbemu::CPU &cpu);

}