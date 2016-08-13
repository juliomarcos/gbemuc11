#include "GPU.hpp"

namespace gbemu {
	
	GPU::~GPU() {
		
	}
	
	void GPU::reallocatePixelsBuffer() {
		// printf ("reallocatePixelsBuffer width %d height %d\n", width, height);
		if (pixels != NULL) {
			delete[] pixels;
		}
		pixels = new byte[width * height * 3];
	}
	
	void GPU::writePixel(int color, int j) {
		// printf("j: %d\n", j);
		//j = -3 + 160*144*3 - j*3;
		j *= 3;
		switch (color) {
			case 0: pixels[j++] = 242; pixels[j++] = 242; pixels[j++] = 242; break;
			case 1: pixels[j++] = 182; pixels[j++] = 182; pixels[j++] = 182; break;
			case 2: pixels[j++] = 96; pixels[j++] = 96; pixels[j++] = 96; break;
			case 3: pixels[j++] = 10; pixels[j++] = 10; pixels[j++] = 10; break;
			default: pixels[j++] = 255; pixels[j++] = 0; pixels[j++] = 144; Log::e(">> VRAM BUFFER ERROR\n"); break;
		}
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
				requiresInterruption = CHECK_BIT(lcdStatus, 4);
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
	
	uint8_t GPU::getColorFromPalette(int paletteAddr, int colorCode) {
		uint8_t palette = cpu.readRam(paletteAddr);
		// printf("palette addr %04x", paletteAddr);
		// cout << " palette " << bitset<8>(palette) << endl;
		return (palette & ((1 << ((colorCode+1)*2))-1)) >> (colorCode*2);
	};
	
	void GPU::drawBackground(uint8_t lcdc) {
		auto scrollX = cpu.scrollX();
		auto scrollY = cpu.scrollY();
		auto windowX = cpu.windowX();
		auto windowY = cpu.windowY();
		const auto scanline = cpu.ly();
		
		auto win = false;
		if (CHECK_BIT(lcdc, WINDOW_DISPLAY_ENABLE)) {
			if (windowY <= scanline) {
				win = true;
			}
		}
		
		int backgroundMap; // this resembles sprites
		if (win) {
			if (!CHECK_BIT(lcdc, WINDOW_TILE_MAP_DISPLAY_SELECT)) {
				backgroundMap = TILE_BG_MAP_1; // 9800
			} else {
				backgroundMap = TILE_BG_MAP_2; // 9C00
			}	
		} else {
			if (!CHECK_BIT(lcdc, BG_TILE_MAP_DISPLAY_SELECT)) {
				backgroundMap = TILE_BG_MAP_1; // 9800
			} else {
				backgroundMap = TILE_BG_MAP_2; // 9C00
			}	
		}
		
		int tilePatternTable;
		bool sign;
		if (CHECK_BIT(lcdc, BG_AND_WINDOW_TILE_DATA_SELECT)) {
			tilePatternTable = TILE_PATTERNS_TABLE_1; // 8000. 0~255
			sign = false;
		} else {
			tilePatternTable = TILE_PATTERNS_TABLE_2; // 9000. -128~127
			sign = true;
		}

		int yPos = win ? scanline-windowY : scrollY+scanline;
		int tileRow = (yPos/8)*32; 
		
		for (int i = 0; i < 160; i++) {
			uint8_t xPos = i + scrollX;
			if (win) {
				if (i > windowX) {
					xPos = i - windowX;
				}
			}
			uint16_t tileDataAddr = tilePatternTable;
			uint8_t tileCol = xPos / 8;
			uint16_t tilePatternAddr = backgroundMap + tileRow + tileCol;
			if (!sign) {
				uint8_t tilePatternNum = cpu.readRam(tilePatternAddr);
				tileDataAddr += tilePatternNum * 16;
			} else {
				int8_t tilePatternNum = (int8_t) cpu.readRam(tilePatternAddr);
				tileDataAddr += tilePatternNum * 16;
			}
							
			uint8_t whichTileLine = (yPos % 8) * 2; // each line in a tile takes 2 bytes
			uint8_t tileLinePart1 = cpu.readRam(tileDataAddr + whichTileLine);
			uint8_t tileLinePart2 = cpu.readRam(tileDataAddr + whichTileLine + 1);
			
			const auto pixelCode = (CHECK_BIT(tileLinePart1, 7-(xPos%8)) ? 1 : 0) | (CHECK_BIT(tileLinePart2, 7-(xPos%8)) ? 2 : 0);
			const auto pixelColor = getColorFromPalette(BGP, pixelCode);
			
			const auto x = i;
			const auto y = scanline;
			if (x < 0 || x > 159 || y < 0 || y > 143) {
				Log::e("ERROR: drawBackground. one is true: x < 0 || x > 159 || y < 0 || y > 143\n");
				Log::e("x %d y %d\n", x, y);
				continue; // ERROR, don't crash the emulator because of that
			}
			writePixel(pixelColor, y*LINE_WIDTH + x);
		}
	}

	
	void GPU::drawSprites(uint8_t lcdc) {
		
		// printf("drawSprites\n");
		
		auto spriteSizeIs8x8 = true;
		if (CHECK_BIT(lcdc, 2)) {
			spriteSizeIs8x8 = false; // it's 8x16
		}
		
		int scanline = cpu.ly();
		// printf("scanline %d\n", scanline);
		
		for(size_t i = 0; i < 40; ++i)
		{
			// printf("drawing sprite nth:%d\n", i);
			
			uint8_t spriteIndex = i * 4;
			auto yPos = cpu.readRam(OAM + spriteIndex) - 16;
			auto xPos = cpu.readRam(OAM + spriteIndex + 1) - 8;
			uint8_t patternIndex = cpu.readRam(OAM + spriteIndex + 2); 
			// TODO: Check from wwhere the following 3 lines cames
			// if (!spriteSizeIs8x8) {
// 				patternIndex = patternIndex >> 1;
// 			}
			// printf ("sprite %d pattern %d x %d y %d\n", i, patternIndex, xPos, yPos);
			uint8_t attributes = cpu.readRam(OAM + spriteIndex + 3);
			auto paletteAddr = CHECK_BIT(attributes, 4) ? OBP1 : OBP0;
			auto xFlip = CHECK_BIT(attributes, 5);
			auto yFlip = CHECK_BIT(attributes, 6);
			
			int ySize = spriteSizeIs8x8 ? 8 : 16;
			
			// only draw sprites portions that belong to this scanline
			if ((scanline >= yPos) && (yPos+ySize)) {
				auto spriteLine = scanline - yPos;
				
				// TODO: yFlip
				
				auto tileAddr = TILE_PATTERNS_TABLE_1 + (patternIndex * 16) + spriteLine*2; // each tile takes 16 bytes
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
					writePixel(pixelColor, y*LINE_WIDTH + x);
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
			Log::i("lcd not enabled\n");
			return;
		}
		if (scanlineDelayCounter > 0) return; 
		
		scanlineDelayCounter = 456;
		auto currentLine = cpu.ly();
		
		if (currentLine < 144) {
			drawScanLine(cpu.lcdc(), currentLine);
		}
		else if (currentLine == 144) {
			Log::i("V_BLANK Started\n");
			interrupt.request(Interrupt::V_BLANK);
		}
		else if (currentLine > 153) { // V-Blank finished
			Log::i("V_BLANK Finished\n");
			cpu.ly(0);
		}
		
		cpu.ly(cpu.ly() + 1);
	}
	
	void GPU::initGraphics() {
		GLuint tex;
		glGenTextures(1, &tex); 
		glBindTexture(GL_TEXTURE_2D, tex);
	}
	
	void GPU::drawPixels() {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBegin(GL_QUADS);
		    glTexCoord2i(0, 0); glVertex2i(0.0f, 0.0f);
		    glTexCoord2i(0, 1); glVertex2i(0.0f, (float)height);
		    glTexCoord2i(1, 1); glVertex2i((float)width, (float)height);
		    glTexCoord2i(1, 0); glVertex2i((float)width, 0);
		glEnd();
	}
	
	void GPU::drawScanLine(uint8_t lcdc, uint8_t currentLine) {
		
		// printf("drawScanLine lcdc %x currentLine %d\n", lcdc, currentLine);
		
		glfwGetFramebufferSize(window, &width, &height);
		if (prevWidth != width) {
			prevWidth = width;
			prevHeight = height;
			reallocatePixelsBuffer();
			glViewport(0.0f, 0.0f, width, height);
		}
		
		if (CHECK_BIT(lcdc, BG_DISPLAY)) {
			drawBackground(lcdc);
		}
		
		if (CHECK_BIT(lcdc, OBJ_SPRITE_DISPLAY_ENABLE)) {
			drawSprites(lcdc);
		}
		
	}
}
