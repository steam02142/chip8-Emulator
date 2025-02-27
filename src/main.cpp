#include "chip8.h"
#include "display.h"
#include <SDL2/SDL.h>

using namespace std;

// The chip8 CPU
Chip8 chip8;

// The screen, with a 1 representing a pixel that is on
bool display[DisplayWidth][DisplayHeight] = {0};

int main(int argc, char** argv)
{
    SDL_Renderer* renderer = initializeDisplay();

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    display[0][0] = 1;
    display[1][2] = 1;

    drawToScreen(renderer, display);
    SDL_Delay(3000);


    chip8.initialize();

    return 0;
}
