#include <GLFW/glfw3.h>
#include "StdLibraries.hpp"
#include "TypeDefinitions.hpp"
#include "BitUtils.hpp"

#include "CPU.hpp"
#include "Interrupt.hpp"
#include "Logger.hpp"

namespace gbemu {
	
	class GPU {
		
		const static int TILE_PATTERNS_TABLE_SPRITE_BG = 0;
		const static int TILE_PATTERNS_TABLE_BG_WINDOW = 0x800;
		const static int TILE_BG_MAP_1 = 0x1800;
		const static int TILE_BG_MAP_2 = 0x1C00;
		const static int OAM = 0xFE00;
		const static int OBP0 = 0xFF48;
		const static int OBP1 = 0xFF49;
		const static int SPRITE_PATTERNS_TABLE = 0x8000;
		
	    const static auto LCD_DISPLAY_ENABLE = 7;
	    const static auto WINDOW_TILE_MAP_DISPLAY_SELECT = 6;
	    const static auto WINDOW_DISPLAY_ENABLE = 5;
	    const static auto BG_AND_WINDOW_TILE_DATA_SELECT = 4;
	    const static auto BG_TILE_MAP_DISPLAY_SELECT = 3;
	    const static auto OBJ_SPRITE_SIZE = 2;
	    const static auto OBJ_SPRITE_DISPLAY_ENABLE = 1;
	    const static auto BG_DISPLAY = 0;
		
		const static auto SCAN_LINE_LENGTH = 456;
		const static auto LINE_WIDTH = 160; // pixels
		
		const static auto H_BLANK = 0;
		const static auto V_BLANK = 1;
		const static auto SEARCHING_SPRITES = 2;
		const static auto TRANSFERING_DATA_LCD = 3;
		
		const static auto MODE_2_BOUND = SCAN_LINE_LENGTH - 80;
		const static auto MODE_3_BOUND = MODE_2_BOUND - 172;
		
		int scanlineDelayCounter;
		GLFWwindow* window;
		CPU& cpu;
		Interrupt& interrupt;
		int width, height;
		int prevWidth, prevHeight;
		byte* pixels;
		
	public:
		GPU(GLFWwindow* window, CPU& cpu, Interrupt& interrupt) : window(window), cpu(cpu), interrupt(interrupt), pixels(NULL) {
		}
		virtual ~GPU();
		
		void draw(int cycles);
		void setLcdStatus();
		void drawScanLine(uint8_t lcdc, uint8_t currentLine);
		void drawWindow(uint8_t lcdc);
		void drawBackground(uint8_t lcdc);
		void drawSprites(uint8_t lcdc);
		void reallocatePixelsBuffer();
		void writePixel(int,int);
		
	};
}