#include <iostream>
#include <cstdint>
#include <string.h>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;

class Chip8{
    public:
        void loadProgram(string path);

        Chip8();
    private:
        // CPU memory
        uint8_t memory[4096];

        // Registers
        uint8_t V0;
        uint8_t V1;
        uint8_t V2;
        uint8_t V3;
        uint8_t V4;
        uint8_t V5;
        uint8_t V6;
        uint8_t V7;
        uint8_t V8;
        uint8_t V9;
        uint8_t VA;
        uint8_t VB;
        uint8_t VC;
        uint8_t VD;
        uint8_t VE;
        uint8_t VF;

        uint16_t I;
        uint8_t delay;
        uint8_t sound;
        
        uint16_t stack[16];
        uint8_t stackPointer;
        uint16_t PC;
};