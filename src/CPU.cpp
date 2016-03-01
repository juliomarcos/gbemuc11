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
		
		// inserts gibberish on VRAM
		// remove this later
		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(0, 0xFF);
		for(size_t i = 0x9FFF; i >= VRAM_START; i--)
		{
			ram[i] = distribution(generator);
		}
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
		//printf("ram[pc] %x %x %x %x\n", ram[pc], ram[pc+1], ram[pc+2], ram[pc+3]);
		//printf("vram start %02x %02x %02x %02x %02x %02x %02x %02x\n", ram[VRAM_START], ram[VRAM_START+1], ram[VRAM_START+2], ram[VRAM_START+3], ram[VRAM_START+4], ram[VRAM_START+5], ram[VRAM_START+6], ram[VRAM_START+7]);
		//printf("vram end %02x %02x %02x %02x %02x %02x %02x %02x\n", ram[VRAM_END], ram[VRAM_END-1], ram[VRAM_END-2], ram[VRAM_END-3], ram[VRAM_END-4], ram[VRAM_END-5], ram[VRAM_END-6], ram[VRAM_END-7]);
		opcode = ram[pc++];
		if (opcode == 0xCB) {
			opcode = (opcode << 8) + ram[pc++];
		}
		//printf("fetched: " ANSI_COLOR_GREEN "%x. " ANSI_COLOR_YELLOW , opcode);
	}
	
	void CPU::decodeAndExecute() {
		switch(opcode) {
			case 0x20:
				//printf("JR NZ,r8");
				duration = jr(NZ, R8);
				break;
			case 0x21:
				//printf("LD HL, d16");
				ld(HL, D16);
				duration = 12;
				break;
			case 0x31: 
				//printf("LD SP, d16");
				ld(SP, D16);
				duration = 12;
				break;
			case 0x32: 
				//printf("LD (HL-), A");
				ldind(HL, SUB, A, NOP);
				duration = 8;
				break;
			case 0xAF:
				//printf("XOR A");
				ixor(A);
				duration = 4;
				break;
			case 0xCB7C:
				//printf("BIT 7, H");
				bit(7, H);
				duration = 8;
				break;
			default: 
				break;
				//printf(">> " ANSI_COLOR_YELLOW "%04x" ANSI_COLOR_RESET " : opcode NOT implemented\n", opcode);
		}
		//printf(ANSI_COLOR_RESET "\n");
	}
	
	uint16_t CPU::hl() {
		return (h<<8) + l;
	}
	
	void CPU::hl(uint16_t vhl) {
		h = vhl >> 8;
		l = (uint8_t) vhl;
	}
	
	uint8_t CPU::scrollX() {
		return ram[0xFF43];
	}
	
	uint8_t CPU::scrollY() {
		return ram[0xFF42];
	}
	
	uint8_t CPU::windowX() {
		return ram[0xFF4A] - 7;
	}
	
	uint8_t CPU::windowY() {
		return ram[0xFF4B];
	}
	
	uint8_t CPU::lcdc() {
		return ram[0xFF40];
	}
	
	void CPU::debugger() {
		// printf("A %x B %x C %x D %x E %x\n", a, b, c, d, e);
		// printf("PC %x SP %x HL %04x\n", pc, sp, hl());
		// bitset<8> bits(f);
		// cout << "F b" << bits << endl;
	}

	byte* CPU::getVramRef() {
		return ram + VRAM_START;
	}

	uint8_t* CPU::getRegisterPointer(Register8 r) {
		switch (r) {
			case A: return &a;
			case B: return &b;
			case C: return &c;
			case D: return &d;
			case E: return &e;
			case F: return &f;
			case H: return &h;
			case L: return &l;
		}
	}
	
	uint16_t* CPU::getRegisterPointer(Register16 r) {
		switch (r) {
			case PC: return &pc;
			case SP: return &sp;
			default: throw invalid_argument("\nRegister passed to getRegisterPointer(Register16) is not supported");
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

	void CPU::ld(Register16 reg, DataType dataType) {
		if (reg == HL) { // HL is not a register, it's a pair of them.
			l = ram[pc++];
			h = ram[pc++];
		} else { 
			uint16_t* regPtr = getRegisterPointer(reg);
			if (dataType == D16) {
				*regPtr = (ram[pc]) + (ram[pc+1] << 8); // little endian
				pc += 2;
			} else {
				*regPtr = ram[pc++];
			}	
		}
	}
	
	void CPU::ldind(Register16 regA, Operation opA, Register8 regB, Operation opB) {
		if (regA == HL) { // special case
			auto vhl = hl();
			ram[vhl] = *getRegisterPointer(regB);
			switch (opA) {
				case SUB: vhl--; break;
			}
			hl(vhl);
		} else {
			// auto *regAptr = getRegisterPointer(regA);
			// auto *regBptr = getRegisterPointer(regB);
		}
	}
	
	void CPU::bit(int whichBit, Register8 reg) {
		auto v = *getRegisterPointer(reg);
		//printf("BIT execution. H %x\n", v);
		if (CHECK_BIT(v, whichBit)) {
			f = f | ZERO_FLAG;
		} else {
			f = f & NEGATE_ZERO_FLAG;
		}
	}
	
	int CPU::jr(Condition cond, DataType dataType) {
		int data;
		switch (dataType) {
			case R8: data = (int8_t) ram[pc++];
		}
		switch (cond) {
			case Z:
				if (!CHECK_BIT(f, 7)) {
					pc = pc+data;	
					return 12;
				} else {
					return 8;
				}
			case NZ:
				if (CHECK_BIT(f, 7)) {
					pc = pc+data;	
					return 12;
				} else {
					return 8;
				}
		}
	}
	
}