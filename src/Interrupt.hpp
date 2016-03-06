#include <cstdint>

#include "CPU.hpp"
#include "BitUtils.hpp"

namespace gbemu {
	
	class Interrupt {
	
		const static int V_BLANK = 0;
		const static int LCD_STAT = 1;
		const static int TIMER = 2;
		const static int SERIAL = 3;
		const static int JOYPAD = 4;
	
		const static int V_BLANK_LOCATION = 0x40;
		const static int LCD_STAT_LOCATION = 0x48;
		const static int TIMER_LOCATION = 0x50;
		const static int SERIAL_LOCATION = 0x58;
		const static int JOYPAD_LOCATION = 0x60; 	
	
		CPU& cpu;
		
	public: 
		
		Interrupt(CPU& cpu) : cpu(cpu) {}
		virtual ~Interrupt() {}
		
		void request(uint8_t which);
		void run();
		void service(uint8_t which);
		
	};
	
}