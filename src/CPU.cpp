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
		printf("ram[pc] %x %x %x %x\n", ram[pc], ram[pc+1], ram[pc+2], ram[pc+3]);
		printf("vram start %02x %02x %02x %02x %02x %02x %02x %02x\n", ram[VRAM_START], ram[VRAM_START+1], ram[VRAM_START+2], ram[VRAM_START+3], ram[VRAM_START+4], ram[VRAM_START+5], ram[VRAM_START+6], ram[VRAM_START+7]);
		printf("vram end %02x %02x %02x %02x %02x %02x %02x %02x\n", ram[VRAM_END], ram[VRAM_END-1], ram[VRAM_END-2], ram[VRAM_END-3], ram[VRAM_END-4], ram[VRAM_END-5], ram[VRAM_END-6], ram[VRAM_END-7]);
		opcode = ram[pc++];
		if (opcode == 0xCB) {
			opcode = (opcode << 8) + ram[pc++];
		}
		printf("fetched: " ANSI_COLOR_GREEN "%x. " ANSI_COLOR_YELLOW , opcode);
	}
	
	void CPU::decodeAndExecute() {
		switch(opcode) {
			case 0x20:
				cout << "JR NZ,r8";
				duration = jr(NZ, R8);
				break;
			case 0x21:
				cout << "LD HL, d16";
				ld(HL, D16);
				duration = 12;
				break;
			case 0x31: 
				cout << "LD SP, d16";
				ld(SP, D16);
				duration = 12;
				break;
			case 0x32: 
				cout << "LD (HL-), A";
				ldind(HL, SUB, A, NOP);
				duration = 8;
				break;
			case 0xAF:
				cout << "XOR A";
				ixor(A);
				duration = 4;
				break;
			case 0xCB7C:
				cout << "BIT 7, H";
				bit(7, H);
				duration = 8;
				break;
			default:
				printf(">> " ANSI_COLOR_YELLOW "%04x" ANSI_COLOR_RESET " : opcode NOT implemented\n", opcode);
		}
		cout << ANSI_COLOR_RESET << endl;
	}
	
	uint16_t CPU::hl() {
		return (h<<8) + l;
	}
	
	void CPU::hl(uint16_t vhl) {
		h = vhl >> 8;
		l = (uint8_t) vhl;
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
		auto *regAptr = getRegisterPointer(regA);
		auto *regBptr = getRegisterPointer(regB);
		
		if (regA == HL) { // special case
			auto vhl = hl();
			ram[vhl] = *regBptr;
			switch (opA) {
				case SUB: vhl--; break;
			}
			hl(vhl);
		} else {
			
		}
		
	}
	
	void CPU::bit(int whichBit, Register8 reg) {
		auto v = *getRegisterPointer(reg);
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
				if (CHECK_BIT(f, 7)) {
					pc = pc+data;	
					return 12;
				} else {
					return 8;
				}
			case NZ:
				if (!CHECK_BIT(f, 7)) {
					pc = pc+data;	
					return 12;
				} else {
					return 8;
				}
		}
	}
	
}