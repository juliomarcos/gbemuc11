#include "CPU.hpp"

namespace gbemu {

	const uint8_t ZERO_FLAG = 128;
	const uint8_t SUBTRACT_FLAG = 64;
	const uint8_t HALF_CARRY_FLAG = 32;
	const uint8_t CARRY_FLAG = 16;
	const uint8_t NEGATE_ZERO_FLAG = 127;

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
			opcode = (opcode << 8) + ram[pc++];
		}
		printf("fetched: %x\n", opcode);
	}
	
	void CPU::decodeAndExecute() {
		switch(opcode) {
			case 0x20:
				duration = jr(NZ, R8);
			case 0x21:
				// cout << "LD HL, dd16";
				ld(HL, 16);
				duration = 12;
			break;
			case 0x31: 
				// cout << "LD SP, dd16" << endl;
				ld(SP, 16);
				duration = 12;
				break;
			case 0x32: 
				// cout << "LD (HL-), A" << endl;
				ldind(HL, SUB, A, NOP);
				duration = 8;
				break;
			case 0xAF:
				// cout << "XOR A" << endl;
				ixor(A);
				duration = 4;
				break;
			case 0xCB7C:
				bit(7, H);
				duration = 8;
				break;
			default:
				printf(">> " ANSI_COLOR_YELLOW "%04x" ANSI_COLOR_RESET " : opcode NOT implemented\n", opcode);
		}
	}
	
	uint16_t CPU::hl() {
		return (h<<8) + l;
	}
	
	void CPU::debugger() {
		printf("A %x B %x C %x D %x E %x\n", a, b, c, d, e);
		printf("PC %x SP %x HL %04x\n", pc, sp, hl());
		bitset<8> bits(f);
		cout << "F b" << bits << endl;
	}

	uint8_t* CPU::getRegisterPointer(Register8 r) {
		switch (r) {
			case A: return &a;
		}
	}
	
	uint16_t* CPU::getRegisterPointer(Register16 r) {
		switch (r) {
			case PC: return &pc;
			case SP: return &sp;
		}
	}

	void CPU::loadRom(shared_ptr<array<byte, CARTRIDGE_SIZE>> bufferPtr)
	{
		rom = *bufferPtr;
		for(size_t i = 0; i < 0x3FF; ++i) // 0x0000-0x3FFF: Permanently-mapped ROM bank.
		{
			ram[i] = rom[i];
		}
	}
	
	void CPU::ixor(Register8 reg) {
		switch (reg) {
			case A:
				a = a ^ a;
				f = (a == 0) ? (ZERO_FLAG | f) : f;
				break;
		}
	}
	//
	// void CPU::jr(Register reg) {
	// 	switch (reg) {
	// 	}
	// }
	//
	void CPU::ld(Register16 reg, int dataSize) {
		if (reg == HL) { // HL is not a register, it's a pair of them.
			l = ram[pc++];
			h = ram[pc++];
		} else { 
			uint16_t* regPtr = getRegisterPointer(reg);
			if (dataSize == 16) {
				*regPtr = (ram[pc]) + (ram[pc+1] << 8); // little endian
				pc += 2;
			} else {
				*regPtr = ram[pc++];
			}	
		}
	}
	
	void CPU::ldind(Register16 regA, Operation opA, Register8 regB, Operation opB) {
		auto *regAptr = getRegisterPointer(regA);
		auto *regBptr = getRegisterPointer(regB);
		
		if (regA == HL) { // special case
			*regBptr = ram[hl()];
			switch (opA) {
				case SUB: l--; break;
			}
		} else {
			
		}
		
	}
	
	void CPU::bit(int whichBit, Register8 reg) {
		auto v = *getRegisterPointer(reg);
		v = v >> whichBit;
		v = v << whichBit;
		if (v > 0) {
			f = f | ZERO_FLAG;
		} else {
			f = f & NEGATE_ZERO_FLAG;
		}
	}
	
	int CPU::jr(Condition cond, DataType dataType) {
		
	}
	
}