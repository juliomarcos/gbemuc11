#pragma once

#include <cstdint>
#include <cstdio>
#include "StdLibraries.hpp"

using byte = unsigned char;

using namespace std;

namespace gbemu {

	const size_t CARTRIDGE_SIZE = 32 * 1024; // TODO: relax this requirement, this is the fixed tetris size
	enum Register { SP };

	class CPU
	{

		byte ram[64 * 1024];
		array<byte, CARTRIDGE_SIZE> rom;
		uint8_t opcode;		

	public:
		CPU();
		virtual ~CPU();
		
		void emulateCycle();
		void fetch();
		void decode();
		void execute();
		
		void load(Register reg, int dataSize);

		void loadRom(shared_ptr<array<byte, CARTRIDGE_SIZE>> buffer);
	
	private:
		uint8_t a, b, c, d, e, f, h, l;
		uint16_t pc, sp;
		
	};

}