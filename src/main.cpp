#include "chip8.h"
#include "display.h"
#include <SDL2/SDL.h>

using namespace std;

// The chip8 CPU
Chip8 chip8;

// The current opcode
uint16_t opcode;

// The screen, with a 1 representing a pixel that is on
bool display[DisplayWidth][DisplayHeight] = {0};

int main(int argc, char** argv)
{
    SDL_Renderer* renderer = initializeDisplay();
    chip8.loadProgram("games/IBM_Logo.ch8");

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    display[0][0] = 1;
    display[1][2] = 1;

    drawToScreen(renderer, display);
    SDL_Delay(3000);

    return 0;
}
