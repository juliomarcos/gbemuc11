#pragma once

#include <cstdint>
#include <cstdio>
#include <bitset>
#include <random>
#include <exception>
#include <string>

#include "StdLibraries.hpp"
#include "ColoredStdOut.hpp"
#include "TypeDefinitions.hpp"
#include "BitUtils.hpp"
#include "Logger.hpp"

using namespace std;

namespace gbemu {

	const size_t CARTRIDGE_SIZE = 32 * 1024; // TODO: relax this requirement, this is the fixed tetris size
	const int CLOCK_SPEED = 4194304;
	
	enum Register8 { A, B, C, D, E, F, H, L };
	enum Register16 { HL, SP, PC, AF, BC, DE };
	enum DataType { D8, D16, A8, A16, R8 };
	enum Operation { ADD, SUB, NOP };
	enum Condition { NZ, Z, NC, CR, NOC }; // CR -> C, C. is already being used
	
	const int VRAM_START = 0x8000;
	const int VRAM_END 	 = 0x9FFF;
	
	class CpuFlags {
		enum WhatToDo {
			Nothing = 0,
			Check = 1 << 0,
			Zero  = 1 << 1,
			One   = 1 << 2
		};
		WhatToDo znhc[4];
	public:
		CpuFlags(string flags) {
			flags.erase(remove_if(	flags.begin(), 
									flags.end(), 
									[](char c){return isspace(c);}),
						flags.end());
			int current = 0;
			for(char& c : flags) {
				if (c == '-') 		znhc[current] = WhatToDo::Nothing;
				else if (c == '1') 	znhc[current] = WhatToDo::One;
				else if (c == '0') 	znhc[current] = WhatToDo::Zero;
				else 				znhc[current] = WhatToDo::Check;
				current += 1;
			}
		}
		bool checkZ() { return znhc[0] == WhatToDo::Check; }
		bool checkN() { return znhc[1] == WhatToDo::Check; }
		bool checkH() { return znhc[2] == WhatToDo::Check; }
		bool checkC() { return znhc[3] == WhatToDo::Check; }
		bool zeroZ() { return znhc[0] == WhatToDo::Zero; }
		bool zeroN() { return znhc[1] == WhatToDo::Zero; }
		bool zeroH() { return znhc[2] == WhatToDo::Zero; }
		bool zeroC() { return znhc[3] == WhatToDo::Zero; }
		bool oneZ() { return znhc[0] == WhatToDo::One; }
		bool oneN() { return znhc[1] == WhatToDo::One; }
		bool oneH() { return znhc[2] == WhatToDo::One; }
		bool oneC() { return znhc[3] == WhatToDo::One; }
	};
	
	class CPU
	{
		array<byte, CARTRIDGE_SIZE> rom;
		uint16_t opcode;		
		int duration;
		int _ime; // Interrupt Master Enable Flag
		uint8_t* getRegisterPointer(Register8 reg);
		uint16_t* getRegisterPointer(Register16 reg);
		int d_count;

	public:
		CPU();
		virtual ~CPU();
		
		byte ram[64 * 1024];
		
		int emulateNextInstruction();
		void fetch();
		void decodeAndExecute();
		void powerUpSequence();
		void debugger();
		void push(uint16_t word);
		uint16_t pop();
		void writeRam(uint16_t address, uint8_t word);
		byte readRam(uint16_t address);
		uint16_t readWord(uint16_t address);
		byte* getVramRef();
		
		uint16_t pc();
		void pc(uint16_t pc);
		uint16_t hl();
		void hl(uint16_t);
		void af(uint16_t);
		void bc(uint16_t);
		void de(uint16_t);
		uint8_t lyc();
		uint8_t ly();
		void ly(uint8_t scanline);
		uint8_t lcdc();
		uint8_t lcdStatus();
		void lcdStatus(uint8_t newStatus);
		uint8_t scrollX();
		uint8_t scrollY();
		uint8_t windowX();
		uint8_t windowY();
		uint8_t ire();
		int ime();
		void ime(int enable);
		uint8_t irflag();
		void irflag(uint8_t flag);
		
		void ld(Register8 reg, DataType dataType);
		void ld(Register8 reg1, Register8 reg2);
		void ld(Register16 reg, DataType dataType);
		void ldind(Register16 regA, Operation opA, Register8 regB, Operation opB);
		void ldind2(Register8 regA, Operation opA, Register16 regB, Operation opB);
		void ldind(Register8 reg1, Register8 reg2);
		void ixor(Register8 reg);
		void bit(int whichBit, Register8 reg);
		int jr(Condition cond, DataType dataType); // returns duration. jumps have different durations according to action or no action taken
		void add(Register8 reg1, Register8 reg2, string flags);
		void add(Register8 reg, uint8_t much, string flags);
		void add(Register16 reg, uint8_t much, string flags);
		void inc(Register8 reg, string flags);
		void inc(Register16 reg, string flags);
		void dec(Register8 reg, string flags);
		void call(DataType dataType);
		void push(Register16);
		void rl(Register8);
		void pop(Register16);
		void ret(Condition cond);
		void cp(DataType dataType);
		void cpind(Register16);
		void ldhE0();
		void ldhF0();
		void ldind(DataType dataType, Register8 reg);
		void sub(Register8 reg);

		void preInitRom();
		void loadRom(vector<char> buffer, int romSize);
	
	private:
		uint8_t a, b, c, d, e, f, h, l;
		uint16_t _pc, _sp;
		
		int dividerAcc;
		
		template<typename T> 
		void setCpuFlags(CpuFlags flags, T oldR1, T reg1Ptr, int reg2Ptr);
	};

}