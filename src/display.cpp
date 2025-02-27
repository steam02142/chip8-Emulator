#include "display.h"

void initializeDisplay(bool display[DisplayHeight][DisplayWidth])
{
    std::cout << "height: " << DisplayHeight << std::endl;

    for(int i = 0; i < DisplayHeight; i++){
        for(int j = 0; j < DisplayWidth; j++) {
            display[i][j] = 0;
        }
    }
}