#include "CPU.hpp"

namespace gbemu {

	const uint8_t ZERO_FLAG = 128;
	const uint8_t SUBTRACT_FLAG = 64;
	const uint8_t HALF_CARRY_FLAG = 32;
	const uint8_t CARRY_FLAG = 16;
	const uint8_t NEGATE_ZERO_FLAG = 127;
		
	const int ZERO_FLAG_POS = 7;
	const int SUBTRACT_FLAG_POS = 6;
	const int HALF_CARRY_FLAG_POS = 5;
	const int CARRY_FLAG_POS = 4;
	
	const uint16_t LY = 0xFF44;
	const uint16_t LYC = 0xFF45;

	CPU::CPU()
	{
		_pc = 0;
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
		// if (d_count++ >= 1 && d_count < 90000) {
		// 	gbemu::Log::currentLogLevel = gbemu::LogLevel::DEBUG;
		if (Log::currentLogLevel == LogLevel::DEBUG) {
			cout << "A " << bitset<8>(a) << "\tB " << bitset<8>(b) << "\n";
			cout << "C " << bitset<8>(c) << "\tD " << bitset<8>(d) << "\n";
			cout << "E " << bitset<8>(e) << "\tF " << bitset<8>(f) << "\n";
			cout << "H " << bitset<8>(h) << "\tL " << bitset<8>(l) << "\n";
			cout << "SP " << bitset<16>(_sp) << endl;
		}
		// if (_pc == 0xE0) {
		// 	gbemu::Log::currentLogLevel = gbemu::LogLevel::DEBUG;
		// }
		// if (_pc== 0xFC) {
		// 	gbemu::Log::currentLogLevel = gbemu::LogLevel::ERROR;
		// }
		// 	printf("ram[pc] %x %x %x %x\n", ram[_pc], ram[_pc+1], ram[_pc+2], ram[_pc+3]);
		// } else {
		// 	gbemu::Log::currentLogLevel = gbemu::LogLevel::NO_LOG;
		// }

		//printf("vram start %02x %02x %02x %02x %02x %02x %02x %02x\n", ram[VRAM_START], ram[VRAM_START+1], ram[VRAM_START+2], ram[VRAM_START+3], ram[VRAM_START+4], ram[VRAM_START+5], ram[VRAM_START+6], ram[VRAM_START+7]);
		//printf("vram end %02x %02x %02x %02x %02x %02x %02x %02x\n", ram[VRAM_END], ram[VRAM_END-1], ram[VRAM_END-2], ram[VRAM_END-3], ram[VRAM_END-4], ram[VRAM_END-5], ram[VRAM_END-6], ram[VRAM_END-7]);
		Log::d(">> %04x ", _pc);
		opcode = ram[_pc++];
		if (opcode == 0xCB) {
			opcode = (opcode << 8) + ram[_pc++];
		}
		// printf("fetched: " ANSI_COLOR_GREEN "%x. " ANSI_COLOR_YELLOW , opcode);
		// printf("fetched: %x. ", opcode);
	}
	
	void CPU::powerUpSequence() {
		_pc = 0x100;
		_sp = 0xFFFE;
		af(0x01B0); // TODO: create this register association
		bc(0x0013); // TODO: create this register association
		de(0x00D8); // TODO: create this register association
		hl(0x014D);
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
	
	void CPU::decodeAndExecute() {
		switch(opcode) {
			case 0x04:
				Log::d("INC B");
				inc(B, "Z 0 H -");
				duration = 4;
				break;
			case 0x05:
				Log::d("DEC B");
				dec(B, "Z 1 H -");
				duration = 4;
				break;
			case 0x06:
				Log::d("LD B,d8");
				ld(B, D8);
				duration = 8;
				break;
			case 0x0c:
				Log::d("INC C");
				inc(C, "Z 0 H -");
				duration = 4;
				break;
			case 0x0d:
				Log::d("DEC C");
				dec(C, "Z 0 H -");
				duration = 4;
				break;
			case 0x0e:
				Log::d("LD C,d8");
				ld(C, D8);
				duration = 8;
				break;
			case 0x11:
				Log::d("LD DE,d16");
				ld(DE, D16);
				duration = 12;
				break;
			case 0x13:
				Log::d("INC DE");
				inc(DE, "- - - -");
				duration = 8;
				break;
			case 0x15:
				Log::d("DEC D");
				dec(D, "- - - -");
				duration = 8;
				break;
			case 0x16:
				Log::d("LD D,d8");
				ld(D, D8);
				duration = 8;
				break;
			case 0x17:
				Log::d("RLA");
				duration = 4;
				rl(A);
				break;
			case 0x18:
				Log::d("JR d8");
				duration = jr(NOC, R8);
				break;
			case 0x1d:
				Log::d("DEC E");
				dec(E, "Z 1 H -");
				duration = 4;
				break;
			case 0x1e:
				Log::d("LD E,d8");
				ld(E, D8);
				duration = 8;
				break;
			case 0x20:
				Log::d("JR NZ,r8");
				duration = jr(NZ, R8);
				break;
			case 0x21:
				Log::d("LD HL,d16");
				ld(HL, D16);
				duration = 12;
				break;
			case 0x22:
				Log::d("LD (HL+),A");
				ldind(HL, ADD, A, NOP);
				duration = 8;
				break;
			case 0x23:
				Log::d("INC HL");
				inc(HL, "- - - -");
				duration = 8;
				break;
			case 0x24:
				Log::d("INC H");
				inc(H, "Z 0 H -");
				duration = 4;
				break;
			case 0x28:
				Log::d("JR Z,r8");
				duration = jr(Z, R8);
				break;
			case 0x2e:
				Log::d("LD L,d8");
				ld(L,D8);
				duration = 8;
				break;
			case 0x31: 
				Log::d("LD SP,d16");
				ld(SP, D16);
				duration = 12;
				break;
			case 0x32: 
				Log::d("LD (HL-),A");
				ldind(HL, SUB, A, NOP);
				duration = 8;
				break;
			case 0x3d:
				Log::d("DEC A");
				dec(A, "Z 1 H -");
				duration = 4;
				break;
			case 0x3e:
				Log::d("LD A,d8");
				ld(A, D8);
				duration = 8;
				break;
			case 0x4f:
				Log::d("LD C,A");
				ld(C, A);
				duration = 4;
				break;
			case 0x57:
				Log::d("LD D,A");
				ld(D,A);
				duration = 8;
				break;
			case 0x67:
				Log::d("LD H,A");
				ld(H,A);
				duration = 8;
				break;
			case 0x77: 
				Log::d("LD (HL),A");
				ldind(HL, NOP, A, NOP);
				duration = 8;
				break;
			case 0x78: 
				Log::d("LD A,B");
				ld(A,B);
				duration = 4;
				break;
			case 0x7b: 
				Log::d("LD A,E");
				ld(A, E);
				duration = 4;
				break;
			case 0x7c: 
				Log::d("LD A,H");
				ld(A, H);
				duration = 4;
				break;
			case 0x7d: 
				Log::d("LD A,L");
				ld(A, L);
				duration = 4;
				break;
			case 0x80:
				Log::d("ADD A,B");
				add(A, B, "Z 0 H C");
				duration = 4;
				break;
			// case 0x86:
// 				Log::d("ADD A,(HL)");
// 				add(A, NOP, HL, NOP);
// 				duration = 8;
// 				break;
			case 0x90:
				Log::d("SUB B");
				sub(B);
				duration = 4;
				break;
			case 0x1a:
				Log::d("LD A,(DE)");
				duration = 8;
				ldind2(A, NOP, DE, NOP);
				break;
			case 0xaf:
				Log::d("XOR A");
				ixor(A);
				duration = 4;
				break;
			case 0xbe:
				Log::d("CP (HL)");
				cpind(HL);
				duration = 8;
				break;
			case 0xc1:
				Log::d("POP BC");
				pop(BC);
				duration = 12;
				break;
			case 0xc5:
				Log::d("PUSH BC");
				push(BC);
				duration = 16;
				break;
			case 0xc9:
				Log::d("RET");
				ret(NOC);
				duration = 16;
				break;
			case 0xcd:
				Log::d("CALL a16");
				call(A16);
				duration = 24;
				break;
			case 0xe0:
				Log::d("LDH (a8),A");
				ldhE0();
				duration = 12;
				break;
			case 0xe2:
				Log::d("LD (C),A");
				ldind(C, A);
				duration = 8;
				break;
			case 0xea:
				Log::d("LD (a16),A");
				ldind(A16,A);
				duration = 16;
				break;
			case 0xf0:
				Log::d("LDH A,(a8)");
				duration = 12;
				ldhF0();
				break;
			case 0xfe:
				Log::d("CP d8");
				cp(D8);
				duration = 8;
				break;
			case 0xcb11:
				Log::d("RL C");
				duration = 8;
				rl(C);
				break;	
			case 0xcb7c:
				Log::d("BIT 7,H");
				bit(7, H);
				duration = 8;
				break;
			default: 
				// printf(">> " ANSI_COLOR_YELLOW "%04x" ANSI_COLOR_RESET " : opcode NOT implemented\n", opcode);
				Log::e(">> %04x : opcode NOT implemented", opcode);
				break;
		}
		// printf(ANSI_COLOR_RESET "\n");
		Log::d("\n");
	}
	
	void CPU::sub(Register8 reg) {
		uint8_t* regPtr = getRegisterPointer(reg);
		uint8_t old = a;
		a -= *regPtr;
		uint8_t newV = -(*regPtr);
		setCpuFlags(CpuFlags("Z 1 H C"), old, a, newV);
	}
	
	void CPU::cp(DataType dataType) {
		// there's only D8
		uint8_t v = -readRam(_pc++);
		Log::d(" 0x%2x", v);
		//printf("CP %4x %4x %4x\n", a, v, a-v);
		uint8_t comp = a + v;
		setCpuFlags(CpuFlags("Z 1 H C"), a, comp, v);
	}
	
	void CPU::cpind(Register16 reg) {
		uint8_t v;
		switch (reg) {
			case HL:
				v = readRam(hl());
				break;
			default:
				Log::e("ERROR: cpind case not impl");
		}
		uint8_t comp = a - v;
		v *= -1;
		setCpuFlags(CpuFlags("Z 1 H C"), a, comp, v);
	}
	
	uint16_t CPU::readWord(uint16_t addr) {
		return readRam(addr) + (readRam(addr+1) << 8);
	}
	
	void CPU::call(DataType dataType) {
		writeRam(--_sp, (_pc+2) >> 8); // b1
		writeRam(--_sp, _pc+2); // b2 works using truncation
		auto nextInstruction = readWord(_pc);
		_pc = nextInstruction;
	}
	
	void CPU::ret(Condition cond) {
		switch (cond) {
			case NOC: break;
			default:
				Log::e(">> ERROR: condition not implemented");
		}
		uint16_t addr = readWord(_sp);
		_sp += 2;
		_pc = addr;
	}
	
	void CPU::rl(Register8 reg) {
		uint8_t* regPtr = getRegisterPointer(reg);
		auto v = *regPtr;
		uint8_t oldCarryFlagMask = (f >> (8-CARRY_FLAG_POS)) & 1;
		uint8_t clearCarryFlag = (f & ~(1 << CARRY_FLAG_POS));
		uint8_t bit7InCarryPosition = (v & (1 << 7)) >> (7-CARRY_FLAG_POS);
		v = (v << 1) | oldCarryFlagMask;
		*regPtr = v;
		if (v == 0) {
			f = RESET_BIT(f, ZERO_FLAG_POS);
		}
		f = clearCarryFlag | bit7InCarryPosition;
	}
	
	void CPU::push(Register16 reg) {
		uint16_t v;
		if (reg == BC) {
			writeRam(--_sp, b);
			writeRam(--_sp, c);
		} else {
			Log::e("push branch not implemented");
		}
	}
	
	void CPU::pop(Register16 reg) {
		switch(reg) {
			case Register16::AF:
				f = readRam(_sp++);
				a = readRam(_sp++);
				break;
			case Register16::BC:
				c = readRam(_sp++);
				b = readRam(_sp++);
				break;
			case Register16::DE:
				e = readRam(_sp++);
				d = readRam(_sp++);
				break;
			case Register16::HL:
				l = readRam(_sp++);
				h = readRam(_sp++);
				break;
		}
	}
	
	template<typename T>	
	void CPU::setCpuFlags(CpuFlags flags, T oldR1, T reg1, int reg2) {
		if (flags.checkZ()) {
			if (reg1 == 0) {
				f = SET_BIT(f, ZERO_FLAG_POS);
			} else {
				f = RESET_BIT(f, ZERO_FLAG_POS);
			}
		}
		if (flags.checkN()) {
			if (reg1 < 0) {
				f = SET_BIT(f, SUBTRACT_FLAG_POS);
			} else {
				f = RESET_BIT(f, SUBTRACT_FLAG_POS);
			}
		}
		if (flags.checkC()) {
			if (oldR1 > 0 && reg2 > 0 && reg1 < 0) {
				f = SET_BIT(f, CARRY_FLAG_POS);
			} 
			if (oldR1 < 0 && reg2 < 0 && reg1 > 0) {
				f = SET_BIT(f, CARRY_FLAG_POS);
			}
		}
		if (flags.checkH()) {
			// TODO: Half Carry
			Log::i(">> ERROR: Half Carry FLAG not implemented");	
		}
		if (flags.zeroZ()) {
			f = RESET_BIT(f, ZERO_FLAG_POS);
		}
		if (flags.zeroN()) {
			f = RESET_BIT(f, SUBTRACT_FLAG_POS);
		}
		if (flags.zeroC()) {
			f = RESET_BIT(f, CARRY_FLAG_POS);
		}
		if (flags.zeroH()) {
			f = RESET_BIT(f, HALF_CARRY_FLAG_POS);
		}
		if (flags.oneZ()) {
			f = SET_BIT(f, ZERO_FLAG_POS);
		}
		if (flags.oneN()) {
			f = SET_BIT(f, SUBTRACT_FLAG_POS);
		}
		if (flags.oneC()) {
			f = SET_BIT(f, CARRY_FLAG_POS);
		}
		if (flags.oneH()) {
			f = SET_BIT(f, HALF_CARRY_FLAG_POS);
		}
	}

	void CPU::add(Register16 reg, int8_t much, string flags) {
		uint16_t oldR1;
		switch (reg) {
			case HL:
				oldR1 = hl();
				hl(hl() + much);
				break;
			default:
				Log::e("add not implemented for this register pair");
				break;
		}
		setCpuFlags(CpuFlags(flags), oldR1, hl(), much);
	}
	
	void CPU::add(Register8 reg1, Register8 reg2, string flags) {
		auto reg1Ptr = getRegisterPointer(reg1);
		auto reg2Ptr = getRegisterPointer(reg2);
		auto oldR1 = *reg1Ptr;
		*reg1Ptr = (*reg1Ptr) + (*reg2Ptr);
		setCpuFlags(CpuFlags(flags), oldR1, *reg1Ptr, *reg2Ptr);
	}
	
	void CPU::inc(Register16 reg, string flags) {
		uint16_t oldR;
		uint16_t v;
		switch (reg) {
			case HL:
				oldR = hl();
				hl(oldR+1);
				v = hl();
				break;
			case DE:
				oldR = (d << 8) + e;
				v = oldR + 1;
				e = (uint8_t) v; // trunc
				d = (v >> 8);
				break;
			default:
				Log::e(">> ERROR: INC nn not implemented");
				
		}
		int one = 1;
		setCpuFlags(CpuFlags(flags), oldR, v, one);
	}
	
	void CPU::inc(Register8 reg, string flags) {
		uint8_t* regPtr = getRegisterPointer(reg);
		uint8_t oldR = *regPtr;
		uint8_t one = 1;
		*regPtr = oldR + one;
		setCpuFlags(CpuFlags(flags), oldR, *regPtr, one);
	}
	
	void CPU::dec(Register8 reg, string flags) {
		uint8_t* regPtr = getRegisterPointer(reg);
		uint8_t oldR = *regPtr;
		uint8_t v = -1;
		*regPtr = oldR + v;
		// cout << " before " << bitset<8>(oldR) << " after " << bitset<8>(*regPtr) << endl;
		setCpuFlags(CpuFlags(flags), oldR, *regPtr, v);
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
	
	void CPU::af(uint16_t afl) {
		a = afl >> 8;
		f = (uint8_t) afl;
	}
	
	void CPU::bc(uint16_t v) {
		b = v >> 8;
		c = (uint8_t) v;
	}
	
	void CPU::de(uint16_t v) {
		d = v >> 8;
		e = (uint8_t) v;
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
		return ram[0xFF4B] - 7;
	}
	
	uint8_t CPU::windowY() {
		return ram[0xFF4A];
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
			if (address==0x28 && value == 0x0f) {
				Log::e("\nERROR WRONG WRONG\n");
			}
			ram[address] = value;
		}
	}
	
	uint8_t CPU::readRam(uint16_t address) {
		// TODO: do several checks here
		return ram[address];
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

	void CPU::loadRom(vector<char> rom, int romSize)
	{
		//rom = *bufferPtr;
		//printf ("ROM SIZE: %d\n", rom.size());
		for(size_t i = 0; i < romSize; ++i) {
			ram[i] = (byte) rom[i];
		}
	}
	
	void CPU::ixor(Register8 reg) {
		int v;
		switch (reg) {
			case A:
				a = 0;
				v = a;
				break;
		}
		if (v == 0) {
			f = (1 << 7);
		}
	}
	
	void CPU::ldhE0() {
		int16_t addr = readRam(_pc++) + 0xff00;
		// printf("ldhE0 addr %4x\n");
		writeRam(addr, a);
		// cout << " ram[0xff40] " << bitset<8>(ram[0xff40]) << endl;
	}

	void CPU::ldhF0() {
		uint8_t n = readRam(_pc++);
		int16_t addr = n + 0xff00;
		a = readRam(addr);
	}

	void CPU::ld(Register8 reg, DataType dataType) {
		uint8_t* regPtr = getRegisterPointer(reg);
		*regPtr = ram[_pc++];	
		Log::d(" %s", bitset<8>(ram[_pc-1]).to_string().c_str());
	}
	
	void CPU::ld(Register8 reg1, Register8 reg2) {
		uint8_t* reg1Ptr = getRegisterPointer(reg1);
		uint8_t* reg2Ptr = getRegisterPointer(reg2);
		*reg1Ptr = *reg2Ptr;
	}

	void CPU::ld(Register16 reg, DataType dataType) {
		Log::d(" %s %s", bitset<8>(ram[_pc]).to_string().c_str(), bitset<8>(ram[_pc+1]).to_string().c_str());
		if (reg == HL) { // HL is not a register, it's a pair of them
			l = ram[_pc++];
			h = ram[_pc++];
		}  else if (reg == SP) {
			auto p = ram[_pc++];
			auto s = ram[_pc++];
			_sp = (s << 8) + p;
		} else if (reg == DE) { // DE is not a register, it's a pair of them
			e = ram[_pc++];
			d = ram[_pc++];
		} else { 
			uint16_t* regPtr = getRegisterPointer(reg);
			if (dataType == D16) {
				*regPtr = readWord(_pc);
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
			Log::d(" RAM[hl-1]: %02x %02x %02x", ram[vhl], ram[vhl+1], ram[vhl+2]);
			switch (opA) {
				case SUB: vhl--; break;
				case ADD: vhl++; break;
			}
			hl(vhl);
		} else {
			Log::e(">> ERROR: ldind reg16 branch not implemented");
			// auto *regAptr = getRegisterPointer(regA);
			// auto *regBptr = getRegisterPointer(regB);
		}
	}
	
	void CPU::ldind(DataType dataType, Register8 reg) {
		if (dataType == A16 && reg == A) {
			int16_t addr = (int16_t) readWord(_pc);
			_pc+=2;
			writeRam(addr, a);	
		} else {
			Log::e("CPU::ldind(DataType dataType, Register8 reg) branch not implemented\n");
		}
	}
	
	void CPU::ldind2(Register8 regA, Operation opA, Register16 regB, Operation opB) {
		uint16_t addr;
		if (regB == BC) {
			auto bPtr = getRegisterPointer(B);
			auto cPtr = getRegisterPointer(C);
			addr = ((*bPtr) << 8) + (*cPtr);
		} else if (regB == DE) {
			auto dPtr = getRegisterPointer(D);
			auto ePtr = getRegisterPointer(E);
			addr = ((*dPtr) << 8) + (*ePtr);
		} else if (regB == HL) {
			addr = hl();
		} else {
			Log::e("error ldind2() branch not implemented");
		}
		*getRegisterPointer(regA) = readRam(addr);
		if (opA != Operation::NOP || opB != Operation::NOP) {
			Log::e("ldind2() branch not implemented");
		}
	}
	
	void CPU::ldind(Register8 reg1, Register8 reg2) {
		if (reg1 == C) {
			writeRam(c+0xff00, a);
		} else {
			Log::e(">> ERROR: ldind reg8 branch not implemented");
		}
	}
	
	void CPU::bit(int whichBit, Register8 reg) {
		auto v = *getRegisterPointer(reg);
		if (!CHECK_BIT(v, whichBit)) {
			f = SET_BIT(f, ZERO_FLAG_POS);
		} else {
			f = RESET_BIT(f, ZERO_FLAG_POS);
		}
		f = RESET_BIT(f, SUBTRACT_FLAG_POS);
		f = SET_BIT(f, HALF_CARRY_FLAG_POS);
	}
	
	int CPU::jr(Condition cond, DataType dataType) {
		int data;
		switch (dataType) {
			case R8: data = (int8_t) ram[_pc++];
		}
		switch (cond) {
			case Z:
				if (CHECK_BIT(f, ZERO_FLAG_POS)) {
					_pc += data;	
					return 12;
				} else {
					return 8;
				}
			case NZ: //F 11110000
				if (!CHECK_BIT(f, ZERO_FLAG_POS)) {
					_pc += data;	
					return 12;
				} else {
					return 8;
				}
			case NOC:
				_pc += data;
				return 12;
			default: 
				Log::e("ERROR: CPU::jr -> Condition case not treated");
				break;
		}
	}
	
}