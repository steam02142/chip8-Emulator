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

unordered_map<char, uint8_t> keymap = {
    {'1', 0x1}, {'2', 0x2}, {'3', 0x3}, {'4', 0xC},
    {'Q', 0x4}, {'W', 0x5}, {'E', 0x6}, {'R', 0xD},
    {'A', 0x7}, {'S', 0x8}, {'D', 0x9}, {'F', 0xE},
    {'Z', 0xA}, {'X', 0x0}, {'C', 0xB}, {'V', 0xF}
};

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
    uint16_t last2Hex = opcode & 0x00FF;
    uint16_t last3Hex = opcode & 0x0FFF;
    uint16_t tempval = 0;


    switch (firstHex)
    {
    case 0x0000: 
        switch (lastHex)
        {
        case 0x0000: // 00E0 (clear screen)

            clearDisplay(renderer, display);
            break;
        case 0x000E: // 00EE (return from subroutine)

            stackPointer--;
            PC = stack[stackPointer];
            
            break;
        
        default:
            cout << "UNKNOWN COMMAND IN 0x0000" << endl;
            break;
        }

        break;
    case 0x1000:

        PC = last3Hex;
        break;
    case 0x2000:

        stack[stackPointer] = PC;
        stackPointer++;
        PC = last3Hex;
        break;
    case 0x3000: 

        tempval = V[((opcode & 0x0F00) >> 8)];
        // Skip instruction if Vx = the second byte in the opcode
        if (tempval == (opcode & 0x00FF)){
            PC += 2;
        }
        break;
    case 0x4000:

        tempval = V[((opcode & 0x0F00) >> 8)];
        // Skip instruction if Vx != the second byte in the opcode
        if (tempval != (opcode & 0x00FF)){
            PC += 2;
        }
        break;
    case 0x5000:

        if(V[((opcode & 0x0F00) >> 8)] == V[((opcode & 0x00F0) >> 4)]){
            PC += 2;
        }
        break;
    case 0x6000:

        // set register VX where X is the second hex digit (which we have to bitshift to get the proper value)
        //      to the value of the last 8 bits
        V[((opcode & 0x0F00) >> 8)] = opcode & 0x00FF;
        break;
    case 0x7000:

        V[((opcode & 0x0F00) >> 8)] += opcode & 0x00FF;
        break;
    // Math and bitwise operations
    case 0x8000:
        switch (lastHex)
        {
        case 0x0000: // Sets Vx = Vy
            V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)];
            break;
        case 0x0001: // Perform bitwise OR on Vx, Vy and store in Vx
            V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x0F00) >> 8)] | V[((opcode & 0x00F0) >> 4)];
            break;
        case 0x0002: // Perform bitwise AND on Vx, Vy and store in Vx
            V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x0F00) >> 8)] & V[((opcode & 0x00F0) >> 4)];
            break;
        case 0x0003: // Perform bitwise XOR on Vx, Vy and store in Vx
            V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x0F00) >> 8)] ^ V[((opcode & 0x00F0) >> 4)];
            break;
        case 0x0004: // Sets Vx = Vx + Vy, if overflow set VF to 1, otherwise 0
            tempval = V[((opcode & 0x0F00) >> 8)] + V[((opcode & 0x00F0) >> 4)];
            V[((opcode & 0x0F00) >> 8)] = tempval;
            if(tempval > 255){
                V[15] = 1;
            } else {
                V[15] = 0;
            }
            break;
        case 0x0005: // Sets Vx = Vx - Vy, if Vx > Vy set VF to 1
            V[15] = 0;
            if (V[((opcode & 0x0F00) >> 8)] > V[((opcode & 0x00F0) >> 4)]){
                V[15] = 1;
            }
            V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x0F00) >> 8)] - V[((opcode & 0x00F0) >> 4)];
            break;
        case 0x0006: // TODO: 
            cout << "TODO\n";

            break;
        case 0x0007: // TODO:
            V[15] = 0;
            if (V[((opcode & 0x00F0) >> 4)] > V[((opcode & 0x0F00) >> 8)]){
                V[15] = 1;
            }
            V[((opcode & 0x0F00) >> 8)] =  V[((opcode & 0x00F0) >> 4)] - V[((opcode & 0x0F00) >> 8)];
            break;
        case 0x000E: // TODO: 
            cout << "TODO\n";
            break;

        
        default:
            break;
        }
        break;
    case 0x9000:
        if(V[((opcode & 0x0F00) >> 8)] != V[((opcode & 0x00F0) >> 4)]){
            PC += 2;
        }
        break;
    case 0xA000:

        I = last3Hex;
        break;
    case 0xB000:
        cout << "B case\n";
        break;
    case 0xC000:
        tempval = randomNumber();

        V[((opcode & 0x0F00) >> 8)] = tempval & (opcode & 0x00FF);
        break;
    case 0xD000:

        Chip8::display(opcode, display);
        break;
    case 0xE000:
        switch (last2Hex)
        {
        case 0x009E: // unsure
            tempval = V[((opcode & 0x0F00) >> 8)];
            if(keysPressed[tempval]){
                PC += 2;
            }
            break;
        case 0x00A1: // unsure
            tempval = V[((opcode & 0x0F00) >> 8)];
            if(!keysPressed[tempval]){
                PC += 2;
            }
            break;
        
        default:
            break;
        }

        break;
    case 0xF000:
        switch (last2Hex)
        {
        case 0x0007: // set Vx to delay timer value
            V[((opcode & 0x0F00) >> 8)] = delay;
            break;
        case 0x000A: // TODO: keypress
            tempval = checkForKeypress();
            if(tempval == 255){
                PC -=2;
            } else{
                V[((opcode & 0x0F00) >> 8)] = tempval;
            }
            break;
        case 0x0015: // set delay timer to Vx
            delay = V[((opcode & 0x0F00) >> 8)];
            break;
        case 0x0018: // set sound timer to Vx
            sound = V[((opcode & 0x0F00) >> 8)];
            break;
        case 0x001E: // Set I = Vx + I
            I = V[((opcode & 0x0F00) >> 8)] + I;
            break;
        case 0x0029: // 
            // Left shift then right shift to get rid of 4 greatest bits
            tempval = (opcode & 0x0F00) << 4;
            tempval = tempval >> 12;
            // Multiply by 5 because there are 5 bytes per character
            I = V[tempval] * 5;

            break;
        case 0x0033: // Store BCD of 1's, 10's, and 100's
            tempval = V[((opcode & 0x0F00) >> 8)];

            memory[I] = (tempval / 100) % 10;
            memory[I + 1] = (tempval / 10) % 10;
            memory[I + 2] = tempval % 10;

            break;
        case 0x0055: // TODO:
            cout << "TODO\n";
            break;
        case 0x0065: // Load X bytes from memory and load them into registers, starting at 0
            tempval = (opcode & 0x0F00) >> 8;
            for(int byte = 0; byte <= tempval; byte++){
                V[byte] = memory[I + byte];
            }
            break;

        
        default:
            break;
        }

        break;
    
    
    default:
        break;
    }    
}

void Chip8::display(uint16_t opcode, bool display[DisplayWidth][DisplayHeight])
{
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;
    int rows = (opcode & 0x000F);
    uint8_t sprite = 0;
    int origx = V[x] % DisplayWidth;
    x = V[x] % DisplayWidth;
    y = V[y] % DisplayHeight;
    V[15] = 0;
    bool bit;

    for(int row = 0; row < rows; row++){
        sprite = memory[I + row];
        x = origx;
        for(int bitShift = 7; bitShift > 0; bitShift--){
            if((x < DisplayWidth) && (y < DisplayHeight)) {
                bit = (sprite >> bitShift) & 0b1;
                display[x][y] = display[x][y] ^ bit;
                x++;
            }
        }
        y++;
    }

}


Chip8::Chip8()
{
    cout << "Initializing CPU\n";

    // Reset memory
    memset(memory, 0, sizeof(memory));

    // Copy the font into the first 80 bits of memory
    copy(font, font + FONT_ARRAY_SIZE, memory);

    
    // Reset registers V0 - VF and set no keys to be pressed
    for(int i = 0; i < 16; i++){
        V[i] = 0;
        keysPressed[i] = 0;
    }

    memset(stack, 0, sizeof(stack));

    I = 0;
    stackPointer = 0;
    // program counter starts where game is loaded in
    PC = 0x200;

    delay = 0;
    sound = 0;
}

// Generate and return a random number from 0-255
int randomNumber()
{
    srand(time(0));

    return rand() % 256;
}

void Chip8::handleInput(SDL_Event event)
{

    SDL_PollEvent(&event);

    if (event.type == SDL_KEYDOWN) {
        storeInput(SDL_GetKeyName(event.key.keysym.sym), true);
    
    } 
    
    if (event.type == SDL_KEYUP) {
        storeInput(SDL_GetKeyName(event.key.keysym.sym), false);
    }


}

// Takes a key and a value to set it to
void Chip8::storeInput(const char* key, bool value)
{
    char betKey = *key;

    unordered_map<char, uint8_t>::const_iterator chip8Key = keymap.find(betKey);

    if(chip8Key != keymap.end()){
        keysPressed[chip8Key->second] = value;
    }
}

uint8_t Chip8::checkForKeypress()
{
    for(int i = 0; i < 16; i++){
        if(keysPressed[i]){
            return i;
        }
    }
    // No key way pressed or it was not valid
    return 255;
}