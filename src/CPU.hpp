#pragma once

#include <cstdint>
#include <cstdio>
#include <bitset>
#include <random>
#include <exception>

#include "StdLibraries.hpp"
#include "ColoredStdOut.hpp"
#include "TypeDefinitions.hpp"
#include "BitUtils.hpp"

using namespace std;

namespace gbemu {

	const size_t CARTRIDGE_SIZE = 32 * 1024; // TODO: relax this requirement, this is the fixed tetris size
	const int CLOCK_SPEED = 4194304;
	
	enum Register8 { A, B, C, D, E, F, H, L };
	enum Register16 { HL, SP, PC, DE };
	enum DataType { D8, D16, A8, A16, R8 };
	enum Operation { ADD, SUB, NOP };
	enum Condition { CR, NC, NZ, Z };
	
	const int VRAM_START = 0x8000;
	const int VRAM_END 	 = 0x9FFF;
	
	class CPU
	{
		array<byte, CARTRIDGE_SIZE> rom;
		uint16_t opcode;		
		int duration;
		int _ime; // Interrupt Master Enable Flag
		uint8_t* getRegisterPointer(Register8 reg);
		uint16_t* getRegisterPointer(Register16 reg);

	public:
		CPU();
		virtual ~CPU();
		
		byte ram[64 * 1024];
		
		int emulateNextInstruction();
		void fetch();
		void decodeAndExecute();
		void debugger();
		void push(uint16_t word);
		uint16_t pop();
		void writeRam(uint16_t address, uint8_t word);
		byte readRam(uint16_t address);
		byte* getVramRef();
		
		uint16_t pc();
		void pc(uint16_t pc);
		uint16_t hl();
		void hl(uint16_t);
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
		void ld(Register16 reg, DataType dataType);
		void ldind(Register16 regA, Operation opA, Register8 regB, Operation opB);
		void ldind(Register8 reg1, Register8 reg2);
		void ixor(Register8 reg);
		void bit(int whichBit, Register8 reg);
		int jr(Condition cond, DataType dataType); // returns duration. jumps have different durations according to action or no action taken
		void add(Register8 reg1, Register8 reg2);

		void preInitRom();
		void loadRom(shared_ptr<array<byte, CARTRIDGE_SIZE>> buffer);
	
	private:
		uint8_t a, b, c, d, e, f, h, l;
		uint16_t _pc, _sp;
		
	};

}