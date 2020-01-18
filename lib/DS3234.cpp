#include "DS3234.h"

DS3234::DS3234()
{
    DS3234Setting = SPISettings(8000000, MSBFIRST, SPI_MODE1);
    SPI.begin();
}

// void DS3234::WriteDataBySPI(uint8_t address, uint8_t data)
// {
//     SPI.beginTransaction(DS3234Setting);
//     digitalWrite(SS, LOW);
//     SPI.transfer(address);
//     SPI.transfer(data);
//     digitalWrite(SS, HIGH);
//     SPI.endTransaction();
// }

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
<<<<<<< HEAD
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
=======
    uint8_t tMonth = month / 10;
    uint8_t month = month % 10;
    WriteDataBySPI(0x85, (tMonth << 4) + month); //I don't know what is "Century"bit in datasheet of ds3234
}

void DS3234::SetDay(int day)
{
    uint8_t tDay = day / 10;
    uint8_t day = day % 10;
    WriteDataBySPI(0x84, (tDay << 4) + day);
}
>>>>>>> 705590f5b9132ff4c8f1e1f2eea075918d077168
