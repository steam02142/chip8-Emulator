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

void Chip8::fetch(uint16_t& opcode)
{
    // Reset opcode
    opcode = 0;

    // Grabs the next two bytes, left shifting the first to make room for the second
    opcode = memory[PC];
    opcode = opcode << 8;
    opcode = opcode | memory[PC + 1];
    PC += 2;

}

void Chip8::decode(uint16_t opcode, SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight])
{
    uint16_t firstHex = opcode & 0xF000;
    uint16_t lastHex = opcode & 0x000F;
    uint16_t last3Hex = opcode & 0x0FFF;

    switch (firstHex)
    {
    case 0x0000: // 00E0 (clear screen)
        cout << "0 case\n";
        clearDisplay(renderer, display);
        break;
    case 0x1000:
        cout << "1 case, jumping from " << PC << " to " << last3Hex << endl;
        PC = last3Hex;
        break;
    case 0x2000:
        cout << "2 case\n";
        break;
    case 0x3000:
        cout << "3 case\n";
        break;
    case 0x4000:
        cout << "4 case\n";
        break;
    case 0x5000:
        cout << "5 case\n";
        break;
    case 0x6000:
        cout << "6 case\n";
        // set register VX where X is the second hex digit (which we have to bitshift to get the proper value)
        //      to the value of the last 8 bits
        V[((opcode & 0x0F00) >> 8)] = opcode & 0x00FF;
        break;
    case 0x7000:
        cout << "7 case\n";
        V[((opcode & 0x0F00) >> 8)] += opcode & 0x00FF;
        break;
    case 0x8000:
        cout << "8 case\n";
        break;
    case 0x9000:
        cout << "9 case\n";
        break;
    case 0xA000:
        cout << "A case\n";
        I = last3Hex;
        break;
    case 0xB000:
        cout << "B case\n";
        break;
    case 0xC000:
        cout << "C case\n";
        break;
    case 0xD000:
        cout << "D case\n";
        break;
    case 0xE000:
        cout << "E case\n";
        break;
    case 0xF000:
        cout << "F case\n";
        break;
    
    
    default:
        break;
    }    
}

Chip8::Chip8()
{
    cout << "Initializing CPU\n";

    // Reset memory
    memset(memory, 0, sizeof(memory));

    // Copy the font into the first 80 bits of memory
    copy(font, font + FONT_ARRAY_SIZE, memory);

    
    // Reset registers V0 - VF
    for(int i = 0; i < 16; i++){
        V[i] = 0;
    }

    memset(stack, 0, sizeof(stack));

    I = 0;
    stackPointer = 0;
    // program counter starts where game is loaded in
    PC = 0x200;

    delay = 0;
    sound = 0;
}
