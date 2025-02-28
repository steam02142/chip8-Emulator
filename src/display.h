#pragma once
#include <iostream>
#include <SDL2/SDL.h>

const int DisplayWidth = 64;
const int DisplayHeight = 32;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

const int PIXEL_SIZE = 10;

SDL_Renderer* initializeDisplay();

void drawToScreen(SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight]);

void clearDisplay(SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight]);