
#include "display.h"

SDL_Renderer* initializeDisplay()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void drawToScreen(SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight])
{
    // Set colour to black and clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set colour to white for drawing pixels that are on
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int xcoord = 0;
    int ycoord = 0;

    for(int x = 0; x < DisplayWidth; x++) {
        for(int y = 0; y < DisplayHeight; y++) {
            if(display[x][y]) { // if the pixel is on
                xcoord = x * PIXEL_SIZE;
                ycoord = y * PIXEL_SIZE;
                SDL_Rect pixel = {xcoord, ycoord, PIXEL_SIZE, PIXEL_SIZE};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(renderer);
}