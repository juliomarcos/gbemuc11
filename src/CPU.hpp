#pragma once

#include <cstdint>
#include <cstdio>
#include <bitset>
#include <cmath>

#include "StdLibraries.hpp"
#include "ColoredStdOut.hpp"

using byte = unsigned char;

using namespace std;

namespace gbemu {

	const size_t CARTRIDGE_SIZE = 32 * 1024; // TODO: relax this requirement, this is the fixed tetris size
	enum Register8 { A, F, B, C, D, E, H, L };
	enum Register16 { HL, SP, PC };
	enum DataType { D8, D16, A8, A16, R8 };
	enum Operation { ADD, SUB, NOP };
	enum Condition { CR, NC, NZ, Z };
	
	class CPU
	{

		byte ram[64 * 1024];
		array<byte, CARTRIDGE_SIZE> rom;
		uint16_t opcode;		
		int duration;
		uint8_t* getRegisterPointer(Register8 reg);
		uint16_t* getRegisterPointer(Register16 reg);

	public:
		CPU();
		virtual ~CPU();
		
		void emulateCycle();
		void fetch();
		void decodeAndExecute();
		void debugger();
		uint16_t hl();
		
		void ld(Register16 reg, int dataSize);
		void ldind(Register16 regA, Operation opA, Register8 regB, Operation opB);
		void ixor(Register8 reg);
		void bit(int whichBit, Register8 reg);
		int jr(Condition cond, DataType dataType); // returns duration. jumps have different durations according to action or no action taken

		void loadRom(shared_ptr<array<byte, CARTRIDGE_SIZE>> buffer);
	
	private:
		uint8_t a, b, c, d, e, f, h, l;
		uint16_t pc, sp;
		
	};

}