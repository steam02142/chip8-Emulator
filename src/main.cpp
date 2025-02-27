#include "chip8.h"
#include "display.h"

using namespace std;

// The chip8 CPU
Chip8 chip8;

// The screen, with a 1 representing a pixel that is on
bool screen[DisplayHeight][DisplayWidth];

int main(int argc, char** argv)
{
    initializeDisplay(screen);
    chip8.initialize();

    return 0;
}
