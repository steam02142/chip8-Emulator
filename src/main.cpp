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

    uint64_t startTime, endTime;

    display[0][0] = 1;
    display[1][2] = 1;

    drawToScreen(renderer, display);
    //SDL_Delay(500);
    int sleepDuration = 0;
    float sixtiethOfASecond = 16.6;

    // Perform fetch, execute, decode cycle 660 times per second
    //      (this is the chip8 instructions per second)
    while(true){
        startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        
        // Fetch, execute, decode
        for(int i = 0; i < 11; i++)
        {
            chip8.fetch(opcode);
            cout << hex << opcode << endl;
            chip8.decode(opcode, renderer, display);
            drawToScreen(renderer, display);
        }

        endTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        // wait for 1/60 of a second (minus the time spent performing instructions)
        sleepDuration = startTime - endTime;
        // usleep is in microseconds, multiply by 1000 for ms
        usleep((sixtiethOfASecond - sleepDuration) * 1000);
    }

    

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    return 0;
}
