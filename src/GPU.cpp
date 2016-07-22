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
	
	void GPU::setLcdStatus() {
		auto lcdStatus = cpu.lcdStatus();
		if (!CHECK_BIT(cpu.lcdc(), LCD_DISPLAY_ENABLE)) {
			scanlineDelayCounter = SCAN_LINE_LENGTH; // it takes 456 cycles to draw one scanline
			cpu.ly(0);
			lcdStatus = SET_BIT(lcdStatus, 0); // set mode to SEARCHING_SPRITES
			lcdStatus = SET_BIT(lcdStatus, 1); // set mode to SEARCHING_SPRITES
			cpu.lcdStatus(lcdStatus);
			return;
		}
		
		auto currentLine = cpu.ly();
		auto currentMode = lcdStatus & 0x3;
		uint8_t newMode = 0;
		auto requiresInterruption = false;
		
		if (currentLine >= 144) { // beginning of a V-Blank
			newMode = V_BLANK;
			lcdStatus = RESET_BIT(lcdStatus, 1);
			lcdStatus = SET_BIT(lcdStatus, 0);
			requiresInterruption = CHECK_BIT(lcdStatus, 4);
		} else {
			if (scanlineDelayCounter >= MODE_2_BOUND) {
				newMode = SEARCHING_SPRITES;
				lcdStatus = RESET_BIT(lcdStatus, 0);
				lcdStatus = SET_BIT(lcdStatus, 1);
				requiresInterruption = CHECK_BIT(lcdStatus, 5);
			} else if (scanlineDelayCounter >= MODE_3_BOUND) {
				newMode = TRANSFERING_DATA_LCD;
				lcdStatus = SET_BIT(lcdStatus, 0);
				lcdStatus = SET_BIT(lcdStatus, 1);
			} else {
				newMode = H_BLANK;
				lcdStatus = RESET_BIT(lcdStatus, 0);
				lcdStatus = RESET_BIT(lcdStatus, 1);
				requiresInterruption = CHECK_BIT(lcdStatus, 3);
			}
		}
		
		if (requiresInterruption && (currentLine != newMode)) {
			interrupt.request(Interrupt::LCD_STATUS);
		}
		
		if (cpu.ly() == cpu.lyc()) {
			lcdStatus = SET_BIT(lcdStatus, 2);
			if (CHECK_BIT(lcdStatus, 6)) {
				interrupt.request(Interrupt::LCD_STATUS);
			}
		} else {
			lcdStatus = RESET_BIT(lcdStatus, 2);
		}
		
		cpu.lcdStatus(lcdStatus);
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
	}
	
	void GPU::drawWindow(uint8_t lcdc) {
		auto windowX = cpu.windowX();
		if (windowX > 166) return;
		auto windowY = cpu.windowY();
	}
	
	void GPU::drawSprites(uint8_t lcdc) {
		auto spriteSizeIs8x8 = true;
		if (CHECK_BIT(lcdc, 2)) {
			spriteSizeIs8x8 = false; // it's 8x16
		}
		
		int scanline = cpu.ly();
		
		auto getColorFromPalette = [&](int paletteAddr, int colorCode) {
			uint8_t palette = cpu.readRam(paletteAddr);
			auto color = palette >> (6-colorCode*2);
			return color << 6;
		};
		
		for(size_t i = 0; i < 40; ++i)
		{
			uint8_t spriteIndex = i * 4;
			auto yPos = cpu.readRam(OAM + spriteIndex) - 16;
			auto xPos = cpu.readRam(OAM + spriteIndex + 1) - 8;
			uint8_t patternIndex = cpu.readRam(OAM + spriteIndex + 2); 
			if (!spriteSizeIs8x8) {
				patternIndex = patternIndex >> 1;
			}
			uint8_t attributes = cpu.readRam(OAM + spriteIndex + 3);
			auto paletteAddr = CHECK_BIT(attributes, 4) ? OBP1 : OBP0;
			auto xFlip = CHECK_BIT(attributes, 5);
			auto yFlip = CHECK_BIT(attributes, 6);
			
			int ySize = spriteSizeIs8x8 ? 8 : 16;
			
			// only draw sprites portions that belong to this scanline
			if (scanline >= yPos && (yPos+ySize)) {
				auto spriteLine = scanline - yPos;
				
				// TODO: yFlip
				
				auto tileAddr = SPRITE_PATTERNS_TABLE + (patternIndex * 16) + spriteLine; // each tile takes 16 bytes
				auto tileLinePart1 = cpu.readRam(tileAddr);
				auto tileLinePart2 = cpu.readRam(tileAddr + 1);

				for(size_t i = 0; i < 8; ++i)
				{
					auto pixelCode = (CHECK_BIT(tileLinePart1, i) ? 1 : 0) + (CHECK_BIT(tileLinePart2, i) ? 2 : 0);
					auto pixelColor = getColorFromPalette(paletteAddr, pixelCode);
					
					// TODO: xFlip
					
					if (pixelColor == 0) continue; // skip white for sprites
					
					auto x = xPos+i;
					auto y = scanline;
					vram[y*LINE_WIDTH + x] = pixelColor;
				}
			}
		}
	}
	
	void GPU::draw(int cycles) {
		
		// printf("draw()\n");
		
		setLcdStatus();
		
		auto lcdc = cpu.lcdc();
		if (CHECK_BIT(lcdc, LCD_DISPLAY_ENABLE)) {
			scanlineDelayCounter -= cycles;
		} else {
			// Log::d("lcd not enabled\n");
			return;
		}
		if (scanlineDelayCounter > 0) return; 
		
		auto currentLine = cpu.ly();
		cpu.ly(currentLine + 1);
		
		cpu.lcdStatus(SEARCHING_SPRITES);
		
		if (currentLine == 144) {
			printf("V_BLANK Started\n");
			interrupt.request(Interrupt::V_BLANK);
		} else if (currentLine > 153) { // V-Blank finished
			printf("V_BLANK Finished\n");
			cpu.ly(0);
		} else {
			drawScanLine(cpu.lcdStatus(), currentLine);
		}
		
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
		
		if (CHECK_BIT(lcdStatus, OBJ_SPRITE_DISPLAY_ENABLE)) {
			drawSprites(lcdStatus);
		}
		
		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, vramToGlBuffer());
	}
}
