#gbemuc11 (looking for a less descriptive name)

This is a gameboy emulator written by a noob in both C++ and emulation, so bare with me.
I'm trying to use modern C++ but I'll also make use of old C if I want to.

Right now the fetch-decode-execute cycle is in place. There's also a non vram mapped window, currently commented.
After all the bootstrap instructions are emulated I'll work on everything that's necessary for the Nintendo logo to scroll the screen.

TODO
[ ]: Log function that can be disabled according to some rule. (number of cycles, some register value, some memory area value, ...)