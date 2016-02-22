#pragma once


#include "StdLibraries.hpp"

typedef unsigned char byte;

using namespace std;

namespace gbemu {

	const size_t CARTRIDGE_SIZE = 32 * 1204;

	class CPU
	{

		byte RAM[8 * 1024];
		byte vRAM[8 * 1024];
		array<byte, CARTRIDGE_SIZE> cart;

	public:
		CPU();
		virtual ~CPU();

		void loadROM(shared_ptr<array<byte, CARTRIDGE_SIZE>> buffer);
		void emulateCycle();
	};

}