#include "../includes/chip8.h"

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

// Mapping of modern day keyboard keys to chip8 keyboard configuration
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

    // Get the number of bytes of the game
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
    uint16_t x = ((opcode & 0x0F00) >> 8);
    uint16_t y = ((opcode & 0x00F0) >> 4);
    uint16_t firstHex = opcode & 0xF000;

    switch (firstHex)
    {
    case 0x0000: 
        handleOpcode0(opcode, renderer, display);
        break;
    case 0x1000:
        handleOpcode1(opcode);
        break;
    case 0x2000:
        handleOpcode2(opcode);
        break;
    case 0x3000: 
        handleOpcode3(opcode, x);
        break;
    case 0x4000:
        handleOpcode4(opcode, x);
        break;
    case 0x5000:
        handleOpcode5(opcode, x, y);
        break;
    case 0x6000:
        handleOpcode6(opcode, x);
        break;
    case 0x7000:
        handleOpcode7(opcode, x);
        break;
    // Math and bitwise operations
    case 0x8000:
        handleOpcode8(opcode, x, y);
        break;
    case 0x9000:
        handleOpcode9(opcode, x, y);
        break;
    case 0xA000:
        handleOpcodeA(opcode);
        break;
    case 0xB000:
        handleOpcodeB(opcode);
        break;
    case 0xC000:
        handleOpcodeC(opcode, x);
        break;
    case 0xD000:
        handleOpcodeD(opcode, display);
        break;
    case 0xE000:
        handleOpcodeE(opcode, x);
        break;
    case 0xF000:
        handleOpcodeF(opcode, x);
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
    bool prevState;

    for(int row = 0; row < rows; row++){
        sprite = memory[I + row];
        x = origx;
        for(int bitShift = 7; bitShift >= 0; bitShift--){
            if((x < DisplayWidth) && (y < DisplayHeight)) {
                bit = (sprite >> bitShift) & 0b1;
                prevState = display[x][y];
                display[x][y] = display[x][y] ^ bit;
                // The bit was on but XORing it turned it off
                if(prevState == 1 && display[x][y] == 0){
                    V[15] = 1;
                }
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

// Polls for keypress
void Chip8::handleInput(SDL_Event event)
{

    if (event.type == SDL_KEYDOWN) {
        storeInput(SDL_GetKeyName(event.key.keysym.sym), true);
    
    } 
    
    if (event.type == SDL_KEYUP) {
        storeInput(SDL_GetKeyName(event.key.keysym.sym), false);
    }
}

// Stores keypress in bool array in the position that matches the key
void Chip8::storeInput(const char* key, bool value)
{
    unordered_map<char, uint8_t>::const_iterator chip8Key = keymap.find(*key);

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

void Chip8::handleOpcode0(uint16_t opcode, SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight])
{
    uint16_t lastHex = opcode & 0x000F;
    switch (lastHex)
        {
        case 0x0000:
            clearDisplay(renderer, display);
            break;
        case 0x000E: // 00EE (return from subroutine)
            stackPointer--;
            PC = stack[stackPointer];
            break;
        
        default:
            cout << "INVALID INSTRUCTION" << endl;
            break;
        }
}

void Chip8::handleOpcode1(uint16_t opcode)
{
    uint16_t last3Hex = opcode & 0x0FFF;
    PC = last3Hex;
}

void Chip8::handleOpcode2(uint16_t opcode)
{
    uint16_t last3Hex = opcode & 0x0FFF;
    stack[stackPointer] = PC;
    stackPointer++;
    PC = last3Hex;
}

void Chip8::handleOpcode3(uint16_t opcode, uint16_t x)
{
    // Skip instruction if Vx = the second byte in the opcode
    if (V[x] == (opcode & 0x00FF)){
        PC += 2;
    }
}

void Chip8::handleOpcode4(uint16_t opcode, uint16_t x)
{
    // Skip instruction if Vx != the second byte in the opcode
    if (V[x] != (opcode & 0x00FF)){
        PC += 2;
    }
}

void Chip8::handleOpcode5(uint16_t opcode, uint16_t x, uint16_t y)
{
    if(V[x] == V[y]){
        PC += 2;
    }
}

void Chip8::handleOpcode6(uint16_t opcode, uint16_t x)
{
    // set register VX where X is the second hex digit (which we have to bitshift to get the proper value)
    //      to the value of the last 8 bits
    V[x] = opcode & 0x00FF;
}

void Chip8::handleOpcode7(uint16_t opcode, uint16_t x)
{
    V[x] += opcode & 0x00FF;
}

void Chip8::handleOpcode8(uint16_t opcode, uint16_t x, uint16_t y)
{
    uint16_t lastHex = opcode & 0x000F;
    uint8_t F = 15;
    switch (lastHex)
        {
        case 0x0000: // Sets Vx = Vy
            V[x] = V[y];
            break;
        case 0x0001: // Perform bitwise OR on Vx, Vy and store in Vx
            V[x] = V[x] | V[y];
            break;
        case 0x0002: // Perform bitwise AND on Vx, Vy and store in Vx
            V[x] = V[x] & V[y];
            break;
        case 0x0003: // Perform bitwise XOR on Vx, Vy and store in Vx
            V[x] = V[x] ^ V[y];
            break;
        case 0x0004: // Sets Vx = Vx + Vy, if overflow set VF to 1, otherwise 0
            V[x] = V[x] + V[y];

            if(V[x] > 255){
                V[F] = 1;
            } else {
                V[F] = 0;
            }
            break;
        case 0x0005: // Sets Vx = Vx - Vy, if Vx > Vy set VF to 1
            V[F] = 0;
            if (V[x] > V[y]){
                V[F] = 1;
            }
            V[x] = V[x] - V[y];
            break;
        case 0x0006: // TODO: 
            V[F] = 0;
            // Perform bitwise and to check if LSB is 1
            if(V[x] & 1){
                V[15] = 1;
            }
            V[x] = V[x] / 2;
            break;
        case 0x0007: 
            if (V[y] > V[x]){
                V[F] = 1;
            }
            V[x] =  V[y] - V[x];
            break;
        case 0x000E:  
            V[F] = 0;
            // Perform bitwise and to check if LSB is 1
            if((V[x] >> 7) & 1){
                V[F] = 1;
            }
            V[x] = V[x] * 2;
            break;
        default:
            break;
        }
}

void Chip8::handleOpcode9(uint16_t opcode, uint16_t x, uint16_t y)
{
    if(V[x] != V[y]){
        PC += 2;
    }
}

void Chip8::handleOpcodeA(uint16_t opcode)
{
    uint16_t last3Hex = opcode & 0x0FFF;
    I = last3Hex;
}

void Chip8::handleOpcodeB(uint16_t opcode)
{
    uint16_t last3Hex = opcode & 0x0FFF;
    PC = last3Hex + V[0];
}

void Chip8::handleOpcodeC(uint16_t opcode, uint16_t x)
{
    uint8_t randNum = randomNumber();
    V[x] = randNum & (opcode & 0x00FF);
}

void Chip8::handleOpcodeD(uint16_t opcode, bool display[DisplayWidth][DisplayHeight])
{
    Chip8::display(opcode, display);
}

void Chip8::handleOpcodeE(uint16_t opcode, uint16_t x)
{
    uint16_t last2Hex = opcode & 0x00FF;
    uint16_t keyCode = V[x];
    switch (last2Hex)
    {
    case 0x009E:
        if(keysPressed[keyCode]){
            PC += 2;
        }
        break;
    case 0x00A1: 
        if(!keysPressed[keyCode]){
            PC += 2;
        }
        break;
    
    default:
        break;
    }
}

void Chip8::handleOpcodeF(uint16_t opcode, uint16_t x)
{
    uint16_t last2Hex = opcode & 0x00FF;
    uint16_t keyPressed = 0;
    uint16_t buffer = 0;
    switch (last2Hex)
    {
    case 0x0007: // set Vx to delay timer value
        V[x] = delay;
        break;
    case 0x000A: // TODO: keypress
        keyPressed = checkForKeypress();
        if(keyPressed == 255){
            PC -=2;
        } else{
            V[x] = keyPressed;
        }
        break;
    case 0x0015: // set delay timer to Vx
        delay = V[x];
        break;
    case 0x0018: // set sound timer to Vx
        sound = V[x];
        break;
    case 0x001E: // Set I = Vx + I
        I = V[x] + I;
        break;
    case 0x0029: // 
        // Left shift then right shift to get rid of 4 greatest bits
        buffer = (opcode & 0x0F00) << 4;
        buffer = buffer >> 12;
        // Multiply by 5 because there are 5 bytes per character
        I = V[buffer] * 5;

        break;
    case 0x0033: // Store BCD of 1's, 10's, and 100's
        buffer = V[x];

        memory[I] = (buffer / 100) % 10;
        memory[I + 1] = (buffer / 10) % 10;
        memory[I + 2] = buffer % 10;

        break;
    case 0x0055: // The values in V0 to Vx are stored in memory starting at I, then I + 1
        for(int i = 0; i <= x; i++){
            memory[I + i] = V[i];
        }
        break;
    case 0x0065: // Load X bytes from memory and load them into registers, starting at 0
        buffer = (opcode & 0x0F00) >> 8;
        for(int byte = 0; byte <= buffer; byte++){
            V[byte] = memory[I + byte];
        }
        break;
    default:
        break;
    }
}