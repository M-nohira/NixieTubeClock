#include "DS3234.h"

DS3234::DS3234()
{
    SPISettings DS3234Setting = SPISettings(8000000, MSBFIRST, SPI_MODE0);
    SPI.begin();
}

void DS3234::WriteDataBySPI(uint8_t address, uint8_t data)
{
    SPI.beginTransaction(DS3234Setting);
    digitalWrite(SS, LOW);
    SPI.transfer(address);
    SPI.transfer(data);
    digitalWrite(SS, HIGH);
    SPI.endTransaction(DS3234Setting);
}

void DS3234::SetYear(int year)
{
    uint8_t tYear = year / 10;
    uint8_t year = year % 10;
    WriteDataBySPI(0x86, (tYear << 4) + year);
}

void DS3234::SetMonth(int month)
{
    uint8_t tMonth = month / 10;
    uint8_t month = month % 10;
    WriteDataBySPI(0x85,(tMonth << 4) + month); //I don't know what is "Century"bit in datasheet of ds3234
}