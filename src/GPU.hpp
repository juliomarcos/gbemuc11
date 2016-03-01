#include <GLFW/glfw3.h>
#include "StdLibraries.hpp"
#include "TypeDefinitions.hpp"
#include "BitUtils.hpp"

#include "CPU.hpp"

namespace gbemu {
	
	class GPU {
		
		const int TILE_SET_1 = 0;
		const int TILE_SET_2 = 0x800;
		const int TILE_MAP_1 = 0x1800;
		const int TILE_MAP_2 = 0x1C00;
		
	    const auto LCD_DISPLAY_ENABLE = 7;
	    const auto WINDOW_TILE_MAP_DISPLAY_SELECT = 6;
	    const auto WINDOW_DISPLAY_ENABLE = 5;
	    const auto BG_AND_WINDOW_TILE_DATA_SELECT = 4;
	    const auto BG_TILE_MAP_DISPLAY_SELECT = 3;
	    const auto OBJ_SPRITE_SIZE = 2;
	    const auto OBJ_SPRITE_DISPLAY_ENABLE = 1;
	    const auto BG_DISPLAY = 0;
		
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
		
		void draw();
		void drawWindow();
		void drawBackground();
		void reallocatePixelsBuffer();
		byte* vramToGlBuffer();
		
	};
}