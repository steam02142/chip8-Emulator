#include "chip8.h"
#include "display.h"
#include <SDL2/SDL.h>
#include <ctime>


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

    std::time_t current_time;

    display[0][0] = 1;
    display[1][2] = 1;

    drawToScreen(renderer, display);
    SDL_Delay(1000);
    
    // Perform fetch, execute, decode cycle 660 times per second
    //      (this is the chip8 instructions per second)
    while(true){
        current_time = std::time(nullptr);
        
        // Fetch, execute, decode
        for(int i = 0; i < 10; i++)
        {
            chip8.fetch(opcode);
            cout << hex << opcode << endl;
            chip8.decode(opcode, renderer, display);
        }

        // wait until the remainder of 1 second has passed
        while(current_time == std::time(nullptr));
    }

    

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    return 0;
}
