#include <Arduino.h>
//Set TubeIndex Shift Register's Pin assign
#define DISPLAY_SER 4  //6
#define DISPLAY_SCLK 3 //5
#define DISPLAY_RCLK 2 //4
//Set show Number
#define NUM_SER 5  //11
#define NUM_SCLK 6 //12
#define NUM_RCLK 8 //14
#define NUM_CLR 7  //13

#define OFF_TIME 250 //Micro Seconds
#define ON_TIME 500 //Micro Seconds

class Nixie
{
private:
    bool isDpShow[6];

    int numCode[11] = {
        0b000000000001, //0
        0b001000000000, //1
        0b000100000000, //2
        0b000010000000, //3
        0b000001000000, //4
        0b000000100000, //5
        0b000000010000, //6
        0b000000001000, //7
        0b000000000100, //8
        0b000000000010,  //9
        0x000//none
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
