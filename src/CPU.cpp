#include "CPU.hpp"

namespace gbemu {

	const uint8_t ZERO_FLAG = 128;
	const uint8_t SUBTRACT_FLAG = 64;
	const uint8_t HALF_CARRY_FLAG = 32;
	const uint8_t CARRY_FLAG = 16;
	const uint8_t NEGATE_ZERO_FLAG = 127;
	
	const uint8_t LY = 0xFF44;
	const uint8_t LYC = 0xFF45;

	CPU::CPU()
	{
		_pc = 0;
		_sp = 0xFFFE;
		// af(0x01B0); // TODO: create this register association
		// bc(0x0013); // TODO: create this register association
		// de(0x00D8); // TODO: create this register association
		hl(0x014D);
		ram[0xFF40] = 0xff;
				
		// inserts gibberish on VRAM
		// TODO: remove this later
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

	int CPU::emulateNextInstruction()
	{
		fetch();
		decodeAndExecute();
		debugger();
		return duration;
	}

	void CPU::fetch() {
		//printf("ram[pc] %x %x %x %x\n", ram[pc], ram[pc+1], ram[pc+2], ram[pc+3]);
		//printf("vram start %02x %02x %02x %02x %02x %02x %02x %02x\n", ram[VRAM_START], ram[VRAM_START+1], ram[VRAM_START+2], ram[VRAM_START+3], ram[VRAM_START+4], ram[VRAM_START+5], ram[VRAM_START+6], ram[VRAM_START+7]);
		//printf("vram end %02x %02x %02x %02x %02x %02x %02x %02x\n", ram[VRAM_END], ram[VRAM_END-1], ram[VRAM_END-2], ram[VRAM_END-3], ram[VRAM_END-4], ram[VRAM_END-5], ram[VRAM_END-6], ram[VRAM_END-7]);
		opcode = ram[_pc++];
		if (opcode == 0xCB) {
			opcode = (opcode << 8) + ram[_pc++];
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
				printf(">> " ANSI_COLOR_YELLOW "%04x" ANSI_COLOR_RESET " : opcode NOT implemented\n", opcode);
		}
		//printf(ANSI_COLOR_RESET "\n");
	}
	
	uint16_t CPU::pc() {
		return _pc;
	}
	
	void CPU::pc(uint16_t pc) {
		_pc = pc;
	}
	
	uint16_t CPU::hl() {
		return (h<<8) + l;
	}
	
	void CPU::hl(uint16_t vhl) {
		h = vhl >> 8;
		l = (uint8_t) vhl;
	}
	uint8_t CPU::lyc() {
		return ram[LYC];
	}
	
	uint8_t CPU::ly() {
		return ram[LY];
	}
	
	void CPU::ly(uint8_t scanline) {
		ram[LY] = scanline; 
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
	
	uint8_t CPU::lcdStatus() {
		return ram[0xFF41];
	}
	
	void CPU::lcdStatus(uint8_t newStatus) {
		ram[0xFF41] = newStatus;
	}
	
	uint8_t CPU::ire() {
		return ram[0xFFFF];
	}
	
	int CPU::ime() {
		return _ime;
	}
	
	void CPU::ime(int enable) {
		_ime = enable;
	}
	
	uint8_t CPU::irflag() {
		return ram[0xFF0F];
	}
	
	void CPU::irflag(uint8_t flag) {
		ram[0xFF0F] = flag;
	}
	
	void CPU::debugger() {
		// printf("A %x B %x C %x D %x E %x\n", a, b, c, d, e);
		// printf("PC %x SP %x HL %04x\n", pc, sp, hl());
		// bitset<8> bits(f);
		// cout << "F b" << bits << endl;
	}
	
	void CPU::writeRam(uint16_t address, uint8_t value) {
		// TODO: do several checks here
		if (address == LY) {
			// reset the current scanline if the game tries to write to it
			ram[address] = 0;
		} else {			
			ram[address] = value;
		}
	}

	void CPU::push(uint16_t word) {
		_sp -= 2;
		writeRam(_sp, word);
		writeRam(_sp+1, (word >> 8));
	}
	
	uint16_t CPU::pop() {
		uint8_t retp1 = ram[_sp];
		uint8_t retp2 = ram[_sp+1];
		_sp += 2;
		return retp1 + (retp2 << 8) ;
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
			case PC: return &_pc;
			case SP: return &_sp;
			default: throw invalid_argument("\nRegister passed to getRegisterPointer(Register16) is not supported");
		}
	}

	void CPU::preInitRom() {
		rom[0xFF05] = 0x00; 
		rom[0xFF06] = 0x00; 
		rom[0xFF07] = 0x00; 
		rom[0xFF10] = 0x80; 
		rom[0xFF11] = 0xBF; 
		rom[0xFF12] = 0xF3; 
		rom[0xFF14] = 0xBF; 
		rom[0xFF16] = 0x3F; 
		rom[0xFF17] = 0x00; 
		rom[0xFF19] = 0xBF; 
		rom[0xFF1A] = 0x7F; 
		rom[0xFF1B] = 0xFF; 
		rom[0xFF1C] = 0x9F; 
		rom[0xFF1E] = 0xBF; 
		rom[0xFF20] = 0xFF; 
		rom[0xFF21] = 0x00; 
		rom[0xFF22] = 0x00; 
		rom[0xFF23] = 0xBF; 
		rom[0xFF24] = 0x77; 
		rom[0xFF25] = 0xF3;
		rom[0xFF26] = 0xF1; 
		rom[0xFF40] = 0x91; 
		rom[0xFF42] = 0x00; 
		rom[0xFF43] = 0x00; 
		rom[0xFF45] = 0x00; 
		rom[0xFF47] = 0xFC; 
		rom[0xFF48] = 0xFF; 
		rom[0xFF49] = 0xFF; 
		rom[0xFF4A] = 0x00; 
		rom[0xFF4B] = 0x00; 
		rom[0xFFFF] = 0x00; 
	}

	void CPU::loadRom(shared_ptr<array<byte, CARTRIDGE_SIZE>> bufferPtr)
	{
		rom = *bufferPtr;
		preInitRom();
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
			l = ram[_pc++];
			h = ram[_pc++];
		} else { 
			uint16_t* regPtr = getRegisterPointer(reg);
			if (dataType == D16) {
				*regPtr = (ram[_pc]) + (ram[_pc+1] << 8); // little endian
				_pc += 2;
			} else {
				*regPtr = ram[_pc++];
			}	
		}
	}
	
	void CPU::ldind(Register16 regA, Operation opA, Register8 regB, Operation opB) {
		if (regA == HL) { // special case
			auto vhl = hl();
			writeRam(vhl, *getRegisterPointer(regB));
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
			case R8: data = (int8_t) ram[_pc++];
		}
		switch (cond) {
			case Z:
				if (!CHECK_BIT(f, 7)) {
					_pc += data;	
					return 12;
				} else {
					return 8;
				}
			case NZ:
				if (CHECK_BIT(f, 7)) {
					_pc += data;	
					return 12;
				} else {
					return 8;
				}
		}
	}
	
}