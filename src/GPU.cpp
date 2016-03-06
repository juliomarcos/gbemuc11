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
	
	// this function is completely non-sensical
	byte* GPU::vramToGlBuffer() {
		auto j = 0;
		for(size_t i = 0; i < (gbemu::VRAM_END - gbemu::VRAM_START); i++) {
			auto color = vram[i];
			switch (color) {
				case 0: pixels[j++] = 242; pixels[j++] = 242; pixels[j++] = 242; break;
				case 1: pixels[j++] = 182; pixels[j++] = 182; pixels[j++] = 182; break;
				case 2: pixels[j++] = 96; pixels[j++] = 96; pixels[j++] = 96; break;
				case 3: pixels[j++] = 10; pixels[j++] = 10; pixels[j++] = 10; break;
				default: pixels[j++] = 10; pixels[j++] = 10; pixels[j++] = 10; printf("VRAM BUFFER ERROR\n"); break;
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
		
		auto lcdc = cpu.lcdc();
	
		if (!CHECK_BIT(lcdc, LCD_DISPLAY_ENABLE)) {
			scanlineDelayCounter -= cycles;
			return;
		}
		
		if (scanlineDelayCounter > 0) return; 
		
		scanlineDelayCounter = 456; // it takes 456 cycles to draw one scanline
		auto currentLine = cpu.ly();
		cpu.ly(currentLine + 1);
		
		if (currentLine == 144) {
			// interrupt.request(Interrupt::V_BLANK)
		} else if (currentLine > 153) { // V-Blank finished
			cpu.ly(0);
		} else {
			drawScanLine(lcdc, currentLine);
		}
		
	}
	
	void GPU::drawScanLine(uint8_t lcdc, uint8_t currentLine) {
		
		printf("drawScanLine lcdc %x currentLine %d\n", lcdc, currentLine);
		
		glfwGetFramebufferSize(window, &width, &height);
		if (prevWidth != width) {
			prevWidth = width;
			prevHeight = height;
			reallocatePixelsBuffer();
		}
		
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

		if (CHECK_BIT(lcdc, WINDOW_DISPLAY_ENABLE)) {
			drawWindow(lcdc);
		}
		
		if (CHECK_BIT(lcdc, BG_DISPLAY)) {
			drawBackground(lcdc);
		}
	}
}