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

<<<<<<< HEAD
<<<<<<< HEAD
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
=======
// void DS3234::WriteDataBySPI(uint8_t address, uint8_t data)
// {
//     SPI.beginTransaction(DS3234Setting);
//     digitalWrite(SS, LOW);
//     SPI.transfer(address);
//     SPI.transfer(data);
//     digitalWrite(SS, HIGH);
//     SPI.endTransaction();
// }
>>>>>>> 705590f5b9132ff4c8f1e1f2eea075918d077168

uint8_t DS3234::ReadDataBySPI(uint8_t address)
=======
void Nixie::ShowDisplay()
>>>>>>> parent of 705590f... update RTC library
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