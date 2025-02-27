#include <iostream>
#include <cstdint>
using namespace std;

class Chip8{
    public:
        void initialize();
    private:
        uint8_t memory[4096];
};