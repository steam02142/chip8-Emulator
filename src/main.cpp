#include "chip8.h"
#include "display.h"
#include <SDL2/SDL.h>
#include <chrono>
#include <unistd.h>


using namespace std;
using namespace std::chrono;

// The chip8 CPU
Chip8 chip8;

// The current opcode
uint16_t opcode;

// The screen, with a 1 representing a pixel that is on
bool display[DisplayWidth][DisplayHeight] = {0};

int main(int argc, char** argv)
{
    SDL_Renderer* renderer = initializeDisplay();
    chip8.loadProgram("games/Pong.ch8");

    // Used to track user input
    SDL_Event event;

    uint64_t startTime, endTime;

    drawToScreen(renderer, display);
    int sleepDuration = 0;
    float sixtiethOfASecond = 16.6;

    // Perform fetch, execute, decode cycle 660 times per second
    //      (this is the chip8 instructions per second)
    while(true){
        startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        
        // Fetch, execute, decode
        for(int i = 0; i < 11; i++)
        {
            chip8.handleInput(event);
            chip8.fetch(opcode);
            //cout << "opcode fetched: " << hex << opcode << endl;
            chip8.decode(opcode, renderer, display);
        }
        
        drawToScreen(renderer, display);

        endTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        // wait for 1/60 of a second (minus the time spent performing instructions)
        sleepDuration = endTime - startTime;
        // usleep is in microseconds, multiply by 1000 for ms
        usleep((sixtiethOfASecond - sleepDuration) * 10);
        chip8.delay--;
        chip8.sound--;
    }

    

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    return 0;
}
