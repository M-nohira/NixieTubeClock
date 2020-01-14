#include <Arduino.h>
#include <SPI.h>

#define SS 16
#define USE_MILITARY_TIME 1 


class DS3234
{
private:
    void WriteDataBySPI(uint8_t data);
public:
    DS3234();
    void SetYear(int year);
    void SetMonth(int month);
    void SetDay(int day);
    void SetHour(int hour);
    void SetMinute(int Minute);
    void SetSecond(int second);
    int GetYear();
    int GetMonth();
    int GetDay();
    int GetHour();
    int GetMinute();
    int GetSecond();
};