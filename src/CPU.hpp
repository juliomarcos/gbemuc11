#pragma once

#include <cstdint>
#include <cstdio>
#include <bitset>
#include "StdLibraries.hpp"

using byte = unsigned char;

using namespace std;

namespace gbemu {

	const size_t CARTRIDGE_SIZE = 32 * 1024; // TODO: relax this requirement, this is the fixed tetris size
	enum Register { A, F, B, C, D, E, H, L, HL, SP, PC };

	class CPU
	{

		byte ram[64 * 1024];
		array<byte, CARTRIDGE_SIZE> rom;
		uint8_t opcode;		
		int duration;

	public:
		CPU();
		virtual ~CPU();
		
		void emulateCycle();
		void fetch();
		void decodeAndExecute();
		void debugger();
		uint16_t hl();
		
		void ld(Register reg, int dataSize);
		void ixor(Register reg);
		// void jr(Register reg);

		void loadRom(shared_ptr<array<byte, CARTRIDGE_SIZE>> buffer);
	
	private:
		uint8_t a, b, c, d, e, f, h, l;
		uint16_t pc, sp;
		
	};

}