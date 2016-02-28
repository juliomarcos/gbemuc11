#include "CPU.hpp"

namespace gbemu {

	CPU::CPU()
	{
		pc = 0;
	}

	CPU::~CPU()
	{
	}

	void CPU::emulateCycle()
	{
		fetch();
		decode();
		execute();
	}

	void CPU::fetch() {
		printf("ram[pc] %x %x %x %x\n", ram[pc], ram[pc+1], ram[pc+2], ram[pc+3]);
		opcode = ram[pc++];
		if (opcode == 0xCB) {
			opcode = opcode + ram[pc++];
		}
		printf("fetched: %x\n", opcode);
	}
	
	void CPU::decode() {
		switch(opcode) {
			case 0x31: 
				cout << "LD SP, dd16" << endl;
				load(SP, 16);
				break;
			default:
				printf(">> %04x: opcode NOT implemented\n", opcode);
		}
	}
	
	void CPU::execute() {
		
	}

	void CPU::loadRom(shared_ptr<array<byte, CARTRIDGE_SIZE>> bufferPtr)
	{
		rom = *bufferPtr;
		for(size_t i = 0; i < 0x3FF; ++i) // 0x0000-0x3FFF: Permanently-mapped ROM bank.
		{
			ram[i] = rom[i];
		}
	}
	
	void CPU::load(Register reg, int dataSize) {
		
	}
	
}