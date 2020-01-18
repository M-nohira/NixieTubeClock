#include "DS3234.h"

DS3234::DS3234()
{
    DS3234Setting = SPISettings(8000000, MSBFIRST, SPI_MODE1);
    SPI.begin();
}

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
{
    SPI.beginTransaction(DS3234Setting);
    digitalWrite(SS, LOW);
    SPI.transfer(address);
    uint8_t result = SPI.transfer(0x00);
    digitalWrite(SS, HIGH);
    SPI.endTransaction();
    return result;
}

int DS3234::SetDateTime(int year, int month, int day, int hour, int min, int sec)
{
    //                 秒,  分,  時間,曜日,日,  月,    年
    int TimeDate[7] = {sec, min, hour, 0, day, month, year};
    for (int i = 0; i <= 6; i++)
    {
        // 3番めは曜日のため代入不要
        if (i == 3)
            continue;

        int b = TimeDate[i] / 10;
        int a = TimeDate[i] - b * 10;

        // if (i == 2)
        // {
        //     if (b == 2)
        //         b  = B00000010;
        //     else if (b == 1)
        //         b = B00000001;
        // }
        TimeDate[i] = a + (b << 4);

        SPI.beginTransaction(DS3234Setting);
        digitalWrite(SS, LOW);
        SPI.transfer(i + 0x80);
        SPI.transfer(TimeDate[i]);
        digitalWrite(SS, HIGH);
        SPI.endTransaction();
    }
}

uint8_t DS3234::GetTime(TimeType type)
{
    uint8_t result = ReadDataBySPI(type);
    if (type == HOUR)
    {
        if (bitRead(result, 6) == 0)
            return (result >> 4) * 10 + (result & 0x0f);
        return bitRead(result, 7) * 12 + ((result >> 4) & 0b001) * 10 + (result & 0x0f);
    }
    if (type == MONTH)
        return bitRead(result, 4) * 10 + (result & 0x0f);
    return (result >> 4) * 10 + (result & 0x0f);
}
