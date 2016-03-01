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
				default: printf("VRAM BUFFER ERROR\n"); break;
			}
		}
		return pixels;
	}
	
	void GPU::drawBackground() {
		auto scrollX = cpu.scrollX();
		auto scrollY = cpu.scrollY();
		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, vramToGlBuffer());
	}
	
	void GPU::drawWindow() {
		
	}
	
	void GPU::draw() {
		
		auto lcdc = cpu.lcdc();
		
		if (!CHECK_BIT(ldcd, LCD_DISPLAY_ENABLE)) {
			return;
		}
		
		glfwGetFramebufferSize(window, &width, &height);
		if (prevWidth != width) {
			prevWidth = width;
			prevHeight = height;
			reallocatePixelsBuffer();
		}
		
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

		if (CHECK_BIT(lcdc, WINDOW_DISPLAY_ENABLE)) {
			drawWindow();
		}
		
		if (CHECK_BIT(lcdc, BG_DISPLAY)) {
			drawBackground();
		}
		
	}
}