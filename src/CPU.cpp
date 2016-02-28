#include "CPU.hpp"

namespace gbemu {

	const uint8_t B10000000 = 128;
	const uint8_t B01000000 = 64;
	const uint8_t B00100000 = 32;
	const uint8_t B00010000 = 16;
	const uint8_t B00001000 = 8;
	const uint8_t B00000100 = 4;
	const uint8_t B00000010 = 2;
	const uint8_t B00000001 = 1;

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
		decodeAndExecute();
		debugger();
	}

	void CPU::fetch() {
		printf("ram[pc] %x %x %x %x\n", ram[pc], ram[pc+1], ram[pc+2], ram[pc+3]);
		opcode = ram[pc++];
		if (opcode == 0xCB) {
			opcode = opcode + ram[pc++];
		}
		printf("fetched: %x\n", opcode);
	}
	
	void CPU::decodeAndExecute() {
		switch(opcode) {
			case 0x21:
				cout << "LD HL, dd16";
				ld(HL, 16);
			break;
			case 0x31: 
				// cout << "LD SP, dd16" << endl;
				ld(SP, 16);
				duration = 12;
				break;
			case 0xAF:
				// cout << "XOR A" << endl;
				ixor(A);
				duration = 4;
				break;
			default:
				printf(">> %04x: opcode NOT implemented\n", opcode);
		}
	}
	
	uint16_t CPU::hl() {
		return (h<<8) + l;
	}
	
	void CPU::debugger() {
		printf("A %x B %x C %x D %x E %x\n", a, b, c, d, e);
		printf("HL %x %x: %04x\n", h, l, hl());
		printf("PC %x SP %x\n", pc, sp);
		bitset<8> bits(f);
		cout << "F b" << bits << endl;
	}

	void CPU::loadRom(shared_ptr<array<byte, CARTRIDGE_SIZE>> bufferPtr)
	{
		rom = *bufferPtr;
		for(size_t i = 0; i < 0x3FF; ++i) // 0x0000-0x3FFF: Permanently-mapped ROM bank.
		{
			ram[i] = rom[i];
		}
	}
	
	void CPU::ixor(Register reg) {
		switch (reg) {
			case A:
				a = a ^ a;
				f = (a == 0) ? (B10000000 | a) : a;
				break;
		}
	}
	//
	// void CPU::jr(Register reg) {
	// 	switch (reg) {
	// 	}
	// }
	//
	void CPU::ld(Register reg, int dataSize) {
		if (reg == HL) { // HL is not a register, it's a pair of them.
			l = ram[pc++];
			h = ram[pc++];
		} else { 
			uint16_t* regPtr;
			switch(reg)	{
				case SP:
					regPtr = &sp;
					break;
			}
			if (dataSize == 16) {
				*regPtr = (ram[pc]) + (ram[pc+1] << 8); // little endian
				pc += 2;
			} else {
				*regPtr = ram[pc++];
			}	
		}
	}
	
}