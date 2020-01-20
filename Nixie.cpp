#include "Nixie.h"

Nixie::Nixie()
{
    //initialize pin 4-6 and 11-14
    for (int cnt = 2; cnt <= 4; cnt++)
    {
        pinMode(cnt, OUTPUT);
        digitalWrite(cnt, 0);
    }
    for (int cnt = 5; cnt <= 8; cnt++)
    {
        pinMode(cnt, OUTPUT);
        digitalWrite(cnt, 0);
    }

    for (int cnt = 0; cnt <= 5; cnt++)
        for (int i = 0; i <= 1; i++)
            schematic[cnt][i] = 0;

    digitalWrite(NUM_CLR, 1);
}

void Nixie::ShowDisplay()
{
    for (int cnt = 0; cnt <= 5; cnt++)
    {
        // index setting
        shiftOut(DISPLAY_SER, DISPLAY_SCLK, MSBFIRST, indexCode[cnt]);
        // num setting
        int code = GetShiftCode(schematic[cnt]);
        shiftOut(NUM_SER, NUM_SCLK, MSBFIRST, (code >> 8));
        shiftOut(NUM_SER, NUM_SCLK, MSBFIRST, code);
        //Activate
        ActivateRegister(NUM_RCLK);
        ActivateRegister(DISPLAY_RCLK);

        // Serial.println(indexCode[cnt], 2);
        // Serial.println(code, 2);

        delayMicroseconds(ON_TIME);
        //Avoid Ghost
        shiftOut(DISPLAY_SER, DISPLAY_SCLK, MSBFIRST, 0x00);
        ActivateRegister(DISPLAY_RCLK);
        delayMicroseconds(OFF_TIME);
    }
    
}

void Nixie::ActivateRegister(int pin_register)
{

    //delay(1);
    digitalWrite(pin_register, 1);
    delayMicroseconds(100);
    digitalWrite(pin_register, 0);
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
        code = (0b01 << 10) + code;
        break;
    case 2:
        code = (0b10 << 10) + code;
        break;
    case 3:
        code = (0b11 << 10) + code;
        break;
    default:
        break;
    }
    return code;
}