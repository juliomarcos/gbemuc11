#include "CPU.hpp"

namespace gbemu {

	CPU::CPU()
	{
	}


	CPU::~CPU()
	{
	}

	void CPU::emulateCycle()
	{
		auto i = 0;
		i++;
	}

	void CPU::loadROM(shared_ptr<array<byte, CARTRIDGE_SIZE>> bufferPtr)
	{
		cart = *bufferPtr;
	}
}