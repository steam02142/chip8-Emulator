#include "../includes/display.h"

SDL_Renderer* initializeDisplay()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
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
            if(display[x][y]) { // if the pixel should be set to on
                xcoord = x * PIXEL_SIZE;
                ycoord = y * PIXEL_SIZE;
                SDL_Rect pixel = {xcoord, ycoord, PIXEL_SIZE, PIXEL_SIZE};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void clearDisplay(SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight])
{
    // Set draw colour to black and clear the display
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Clear the values of the display
    for(int x = 0; x < DisplayWidth; x++) {
        for(int y = 0; y < DisplayHeight; y++) {
            display[x][y] = 0;
        }
    }

    // Render our changes (cleared screen)
    SDL_RenderPresent(renderer);
}