#include "Interrupt.hpp"

namespace gbemu {
	
	void Interrupt::request(uint8_t which) {
		auto irflag = cpu.irflag();
		irflag = SET_BIT(irflag, which);
		cpu.irflag(irflag);
	}
	
	void Interrupt::run() {
		auto ime = cpu.ime();
		if (ime == 0) {
			return;
		}
		auto which = cpu.irflag();
		auto ire = cpu.ire();
		for(size_t i = 0; i < 5; ++i) {
			if (CHECK_BIT(ire, i)) {
				service(i);
			}
		}
	}
		
	void Interrupt::service(uint8_t which) {
		cpu.ime(false);
		auto irflag = cpu.irflag();
		irflag = RESET_BIT(irflag, which);
		cpu.irflag(irflag);
		cpu.push(cpu.pc());
		switch(which) {
			case 0: cpu.pc(V_BLANK_LOCATION); break;
			case 1: cpu.pc(LCD_STAT_LOCATION); break;
			case 2: cpu.pc(TIMER_LOCATION); break;
			case 3: cpu.pc(SERIAL_LOCATION); break;
			case 4: cpu.pc(JOYPAD_LOCATION ); break;
		}
	}
}