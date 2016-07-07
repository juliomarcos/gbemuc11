#include "GPU.hpp"

namespace gbemu {
	
	GPU::~GPU() {
		
	}
	
	void GPU::reallocatePixelsBuffer() {
		if (pixels != NULL) {
			delete[] pixels;
		}
		pixels = new byte[width * height * 3];
	}

	byte* GPU::vramToGlBuffer() {
		auto j = 0;
		for(size_t i = 0; i < (gbemu::VRAM_END - gbemu::VRAM_START); i++) {
			auto color = vram[i];
			switch (color) {
				case 0: pixels[j++] = 242; pixels[j++] = 242; pixels[j++] = 242; break;
				case 1: pixels[j++] = 182; pixels[j++] = 182; pixels[j++] = 182; break;
				case 2: pixels[j++] = 96; pixels[j++] = 96; pixels[j++] = 96; break;
				case 3: pixels[j++] = 10; pixels[j++] = 10; pixels[j++] = 10; break;
				default: pixels[j++] = 255; pixels[j++] = 0; pixels[j++] = 144; // printf("VRAM BUFFER ERROR\n"); break;
			}
		}
		return pixels;
	}
	
	void GPU::drawBackground(uint8_t lcdc) {
		auto scrollX = cpu.scrollX();
		auto scrollY = cpu.scrollY();
		int bgTileMapOffset;
		if (!CHECK_BIT(lcdc, BG_TILE_MAP_DISPLAY_SELECT)) {
			bgTileMapOffset = TILE_BG_MAP_1; // 0~255
		} else {
			bgTileMapOffset = TILE_BG_MAP_2; // -128~127
		}
		
		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, vramToGlBuffer());
	}
	
	void GPU::drawWindow(uint8_t lcdc) {
		auto windowX = cpu.windowX();
		if (windowX > 166) return;
		auto windowY = cpu.windowY();
	}
	
	void GPU::draw(int cycles) {
		
		setLcdStatus();
		
		auto lcdStatus = cpu.lcdStatus();
		if (CHECK_BIT(lcdStatus, LCD_DISPLAY_ENABLE)) {
			scanlineDelayCounter -= cycles;
		} else {
			return;
		}
		if (scanlineDelayCounter > 0) return; 
		
		auto currentLine = cpu.ly();
		cpu.ly(currentLine + 1);
		
		cpu.lcdStatus(SEARCHING_SPRITES);
		
		if (currentLine == 144) {
			// printf("V_BLANK Started\n");
			interrupt.request(Interrupt::V_BLANK);
		} else if (currentLine > 153) { // V-Blank finished
			// printf("V_BLANK Finished\n");
			cpu.ly(0);
		} else {
			drawScanLine(lcdStatus, currentLine);
		}
		
	}
	
	void GPU::setLcdStatus() {
		auto lcdStatus = cpu.lcdStatus();
		if (!CHECK_BIT(lcdStatus, LCD_DISPLAY_ENABLE)) {
			scanlineDelayCounter = SCAN_LINE_LENGTH; // it takes 456 cycles to draw one scanline
			cpu.ly(0);
			SET_BIT(lcdStatus, 0); // set mode to SEARCHING_SPRITES
			SET_BIT(lcdStatus, 1); // set mode to SEARCHING_SPRITES
			cpu.lcdStatus(lcdStatus);
			return;
		}
		
		auto currentLine = cpu.ly();
		auto currentMode = lcdStatus & 0x3;
		uint8_t newMode = 0;
		auto requiresInterruption = false;
		
		if (currentLine >= 144) { // beginning of a V-Blank
			newMode = V_BLANK;
			RESET_BIT(lcdStatus, 1);
			SET_BIT(lcdStatus, 0);
			requiresInterruption = CHECK_BIT(lcdStatus, 4);
		} else {
			if (scanlineDelayCounter >= MODE_2_BOUND) {
				newMode = SEARCHING_SPRITES;
				RESET_BIT(lcdStatus, 0);
				SET_BIT(lcdStatus, 1);
				requiresInterruption = CHECK_BIT(lcdStatus, 5);
			} else if (scanlineDelayCounter >= MODE_3_BOUND) {
				newMode = TRANSFERING_DATA_LCD;
				SET_BIT(lcdStatus, 0);
				SET_BIT(lcdStatus, 1);
			} else {
				newMode = H_BLANK;
				RESET_BIT(lcdStatus, 0);
				RESET_BIT(lcdStatus, 1);
				requiresInterruption = CHECK_BIT(lcdStatus, 3);
			}
		}
		
		if (requiresInterruption && (currentLine != newMode)) {
			interrupt.request(Interrupt::LCD_STATUS);
		}
		
		if (cpu.ly() == cpu.lyc()) {
			SET_BIT(lcdStatus, 2);
			if (CHECK_BIT(lcdStatus, 6)) {
				interrupt.request(Interrupt::LCD_STATUS);
			}
		} else {
			RESET_BIT(lcdStatus, 2);
		}
		
		cpu.lcdStatus(lcdStatus);
	}
	
	void GPU::drawScanLine(uint8_t lcdStatus, uint8_t currentLine) {
		
		// printf("drawScanLine lcdc %x currentLine %d\n", lcdc, currentLine);
		
		glfwGetFramebufferSize(window, &width, &height);
		if (prevWidth != width) {
			prevWidth = width;
			prevHeight = height;
			reallocatePixelsBuffer();
		}
		
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

		if (CHECK_BIT(lcdStatus, WINDOW_DISPLAY_ENABLE)) {
			drawWindow(lcdStatus);
		}
		
		if (CHECK_BIT(lcdStatus, BG_DISPLAY)) {
			drawBackground(lcdStatus);
		}
	}
}