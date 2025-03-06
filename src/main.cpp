#include "../includes/chip8.h"
#include "../includes/display.h"
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
    string game;
    if (argv[1]){
        game = argv[1];
    } else {
        game = "Pong.ch8";
    }
    

    SDL_Renderer* renderer = initializeDisplay();
    chip8.loadProgram("games/" + game);

    // Used to track user input
    SDL_Event event;

    uint64_t startTime, endTime;

    drawToScreen(renderer, display);
    int sleepDuration = 0;
    float sixtiethOfASecond = 16.6;

    bool running = true;

    // Perform fetch, execute, decode cycle 660 times per second
    //      (this is the chip8 instructions per second)
    while(running){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            chip8.handleInput(event);
        }

        startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        
        // Fetch, execute, decode
        for(int i = 0; i < 11; i++)
        {
            chip8.handleInput(event);
            chip8.fetch(opcode);
            //cout << "Opcode: " << opcode << endl;
            chip8.decode(opcode, renderer, display);
        }

        drawToScreen(renderer, display);

        endTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        // wait for 1/60 of a second (minus the time spent performing instructions)
        sleepDuration = endTime - startTime;
        usleep((sixtiethOfASecond - sleepDuration) * 1000);
        chip8.delay--;
        chip8.sound--;

    }

    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
   

    return 0;
}
