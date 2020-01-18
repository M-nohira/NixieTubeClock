#include <Arduino.h>
//Set TubeIndex Shift Register's Pin assign
#define DISPLAY_SER 6
#define DISPLAY_SCLK 5
#define DISPLAY_RCLK 4
//Set show Number
#define NUM_SER 11
#define NUM_SCLK 12
#define NUM_RCLK 14
#define NUM_CLR 13

#define OUTPUT 0x1

class Nixie
{
private:
    
    bool isDpShow[6];

    int numCode[10] = {
        0b001000000000 ,
        0b000000000001,
        0b000000000010,
        0b000000000100,
        0b000000001000,
        0b000000010000,
        0b000000100000,
        0b000001000000,
        0b000010000000,
        0b000100000000
    };

    uint8_t indexCode[6] = {
        0b00000001,
        0b00000010,
        0b00000100,
        0b00001000,
        0b00010000,
        0b00100000,
    };

    int GetShiftCode(int number[2]);
    int JudgeDpCode(int code, int dpcode);
    void ActivateRegister(int pin_register);

public:
    int schematic[6][2]; //[0] is Numerial 0-9 / [1] is dp 0=none 1=ldp 2=rdp 3=both

    Nixie();
    void ShowDisplay();
};
