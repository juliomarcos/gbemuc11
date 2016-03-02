#include <GLFW/glfw3.h>
#include "StdLibraries.hpp"
#include "TypeDefinitions.hpp"
#include "BitUtils.hpp"

#include "CPU.hpp"

namespace gbemu {
	
	class GPU {
		
		const static int TILE_SET_1 = 0;
		const static int TILE_SET_2 = 0x800;
		const static int TILE_MAP_1 = 0x1800;
		const static int TILE_MAP_2 = 0x1C00;
		
	    const static auto LCD_DISPLAY_ENABLE = 7;
	    const static auto WINDOW_TILE_MAP_DISPLAY_SELECT = 6;
	    const static auto WINDOW_DISPLAY_ENABLE = 5;
	    const static auto BG_AND_WINDOW_TILE_DATA_SELECT = 4;
	    const static auto BG_TILE_MAP_DISPLAY_SELECT = 3;
	    const static auto OBJ_SPRITE_SIZE = 2;
	    const static auto OBJ_SPRITE_DISPLAY_ENABLE = 1;
	    const static auto BG_DISPLAY = 0;
		
		int scanlineDelayCounter;
		GLFWwindow* window;
		CPU& cpu;
		byte* vram;
		int width, height;
		int prevWidth, prevHeight;
		byte* pixels;
		
	public:
		GPU(GLFWwindow* window, CPU& cpu) : window(window), cpu(cpu), pixels(NULL) {
			vram = cpu.ram+gbemu::VRAM_START;
		}
		virtual ~GPU();
		
		void draw(int cycles);
		void drawScanLine(uint8_t lcdc, uint8_t currentLine);
		void drawWindow();
		void drawBackground();
		void reallocatePixelsBuffer();
		byte* vramToGlBuffer();
		
	};
}