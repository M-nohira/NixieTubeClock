#include "Nixie.h"

Nixie::Nixie()
{
    //initialize pin 4-6 and 11-14
    for (int cnt = 4; cnt <= 6; cnt++)
        pinMode(OUTPUT, cnt);
    for (int cnt = 11; cnt <= 14; cnt++)
        pinMode(OUTPUT, cnt);

    for (int cnt = 0; cnt <= 6; cnt++)
        for (int i = 0; i <= 1; i++)
            schematic[cnt][i] = 0;
}

void Nixie::ShowDisplay()
{
    for (int cnt = 0; cnt <= 6; cnt++)
    {
        // num setting
        int code = GetShiftCode(schematic[cnt]);
        shiftOut(NUM_SER, NUM_SCLK, MSBFIRST, (code >> 8));
        shiftOut(NUM_SER, NUM_SCLK, MSBFIRST, code);
        ActivateRegister(NUM_RCLK);
        // index setting
        shiftOut(DISPLAY_SER, DISPLAY_SCLK, MSBFIRST, indexCode[cnt]);
        ActivateRegister(DISPLAY_RCLK);
    }
}

void Nixie::ShowDisplay()
{
    for (int cnt = 0; cnt <= 6; cnt++)
    {
        //num setting
        int code = GetShiftCode(schematic[cnt]);
        shiftOut(NUM_SER, NUM_SCLK, MSBFIRST, (code >> 8));
        shiftOut(NUM_SER, NUM_SCLK, MSBFIRST, code);
        ActivateRegister(NUM_RCLK);
        //index setting
        shiftOut(DISPLAY_SER, DISPLAY_SCLK, MSBFIRST, indexCode[cnt]);
        ActivateRegister(DISPLAY_RCLK);
    }
}

void Nixie::ActivateRegister(int pin_register)
{
    digitalWrite(pin_register, 0);
    //delay(1);
    digitalWrite(pin_register, 1);
    //delay(1);
}

int Nixie::GetShiftCode(int number[2])
{
    int result = 0;
    result = JudgeDpCode(numCode[number[0]], number[1]);
    return result;
}

int Nixie::JudgeDpCode(int code, int dpcode)
{
    switch (dpcode)
    {
    case 1:
        code = (0b01 << 10) or code;
        break;
    case 2:
        code = (0b10 << 10) or code;
        break;
    case 3:
        code = (0b11 << 10) or code;
        break;
    default:
        break;
    }
    return code;
}