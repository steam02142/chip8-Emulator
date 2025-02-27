#include "chip8.h"
#include <bitset>

const int FONT_ARRAY_SIZE = 80;

// Each row of 5 hex values corresponds to the binary to make characters 
//      from 0-9 and A-F
uint8_t font[FONT_ARRAY_SIZE] = {0xF0, 0x90, 0x90, 0x90, 0xF0, 
0x20, 0x60, 0x20, 0x20, 0x70, 
0xF0, 0x10, 0xF0, 0x80, 0xF0, 
0xF0, 0x10, 0xF0, 0x10, 0xF0, 
0x90, 0x90, 0xF0, 0x10, 0x10, 
0xF0, 0x80, 0xF0, 0x10, 0xF0, 
0xF0, 0x80, 0xF0, 0x90, 0xF0, 
0xF0, 0x10, 0x20, 0x40, 0x40, 
0xF0, 0x90, 0xF0, 0x90, 0xF0, 
0xF0, 0x90, 0xF0, 0x10, 0xF0, 
0xF0, 0x90, 0xF0, 0x90, 0x90, 
0xE0, 0x90, 0xE0, 0x90, 0xE0, 
0xF0, 0x80, 0x80, 0x80, 0xF0, 
0xE0, 0x90, 0x90, 0x90, 0xE0, 
0xF0, 0x80, 0xF0, 0x80, 0xF0, 
0xF0, 0x80, 0xF0, 0x80, 0x80  };

// Loads a program at 'path' to memory at location 0x200
void Chip8::loadProgram(string path)
{
    cout << "Loading program " << path << endl;
    ifstream game(path, ios::binary);
    if(!game) {
        cout << "Error: unable to open program at " << path << endl;
    }
    long fileSize = filesystem::file_size(path);
    cout << "File size: " << fileSize << endl;

    // Get the bytes of the game
    char buffer [fileSize];
    memset(buffer, 0, sizeof(buffer));
    game.read(buffer, fileSize);

    copy(reinterpret_cast<uint8_t*>(buffer), reinterpret_cast<uint8_t*>(buffer) + fileSize, memory + 0x200);
}

Chip8::Chip8()
{
    cout << "Initializing CPU\n";

    // Reset memory
    memset(memory, 0, sizeof(memory));

    // Copy the font into the first 80 bits of memory
    copy(font, font + FONT_ARRAY_SIZE, memory);

    
    // Reset registers V0 - VF
    V0 = 0;
    V1 = 0;
    V2 = 0;
    V3 = 0;
    V4 = 0;
    V5 = 0;
    V6 = 0;
    V7 = 0;
    V8 = 0;
    V9 = 0;
    VA = 0;
    VB = 0;
    VC = 0;
    VD = 0;
    VE = 0;
    VF = 0;

    memset(stack, 0, sizeof(stack));

    I = 0;
    stackPointer = 0;
    // program counter starts where game is loaded in
    PC = 0x200;

    delay = 0;
    sound = 0;
}