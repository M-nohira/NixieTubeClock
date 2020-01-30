
#include "DS3234.h"
#include "Nixie.h"
#include "DriveBoard.h"
//PIN ASSIGN

//MODE SETTINGS
#define MODE_SERIAL 2
#define MODE_SETTING 1
#define MODE_DISPLAY 0

//fLASHING SETTINGS
#define RTC_READ_PERIOD 100 //milli seconds

#define KEY_FLAG_STATE 0 // 0=pressed 1=pressing

DS3234 rtc;
Nixie nixie;
DriveBoard boad;
unsigned long elaped_time = 0;

int time[7] = {0, 0, 0, 0, 0, 0};
int _mode = 0;
int setting_index = 6;

void mode_Clock(uint8_t keyState);
void mode_SetTime(uint8_t keyState);
void mode_SerialSetTime(uint8_t keyState);

void ReadCurrentTime();
void ApplyRTCTime();
void SetUpTimeBySerial(String str);
void SetNixieSchematicByTime(int *code, int *dpCode, bool none = false);
void LimitDateValue(int parameter, int max, int min);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    uint8_t keyState = boad.ReadKeyState();
    if (Serial.available() > 0)
    {
        _mode = MODE_SERIAL;
    }
    if(_mode == MODE_DISPLAY) mode_Clock(keyState);
    if(_mode == MODE_SETTING) mode_SetTime(keyState);
    if(_mode == MODE_SERIAL) mode_SerialSetTime(keyState);
    nixie.ShowDisplay();
}

void mode_Clock(uint8_t keyState)
{
    ReadCurrentTime();

    int sw_bFlag = bitRead(keyState, 8);
    _mode = MODE_SETTING;

    int dp[6] = {0, 1, 0, 1, 0, 1};
    SetNixieSchematicByTime(time, dp);
}

void mode_SetTime(uint8_t keyState)
{
    if (bitRead(keyState, 7)) //Back Flag
    {
        if (setting_index == 6)
            setting_index == setting_index;
        setting_index = setting_index == 2 ? setting_index + 2 : setting_index + 1;
    }

    if (bitRead(keyState, 6)) //Accept Flag
    {
        if (setting_index == 1)
        {
            ApplyRTCTime();
            _mode = MODE_DISPLAY;
        }
        setting_index = setting_index == 4 ? setting_index - 2 : setting_index - 1;
    }

    if (bitRead(keyState, 5)) //Minus Flag
        time[setting_index]--;

    if (bitRead(keyState, 4)) //Plus Flag
        time[setting_index]++;

    LimitDateValue(SECOND, 59, 0);
    LimitDateValue(MINUTE, 59, 0);
    LimitDateValue(HOUR, 23, 0);
    LimitDateValue(DAY, 31, 1);
    LimitDateValue(MONTH, 12, 1);
    LimitDateValue(YEAR, 99, 0);

    int dp[6] = {0, 1, 0, 1, 0, 1};

    if (setting_index > 3)
    {
        int tt[3] = {10, 10, 10};
        SetNixieSchematicByTime(tt, dp);
        return;
    }
    int tt[3] = {time[HOUR], time[MINUTE], time[SECOND]};
    SetNixieSchematicByTime(tt, dp);
}

void mode_SerialSetTime(uint8_t keyState)
{
    Serial.println("SERIAL MODE ENTER COMMAND");
    char command = Serial.read();
    switch (command)
    {
    case 's':
        Serial.println("SETTING TIME ENTER 6BYTES TIME CODE");
        uint8_t buffer[6];
        Serial.readBytes(buffer, sizeof(buffer));
        for (int i = 0; i <= 5; i++)
            time[i] = buffer[i];
        break;
    case 'q':
        Serial.println("QUIT SERIAL MODE");
        _mode = MODE_DISPLAY;
        break;
    case 'a':
        Serial.println("APPLYED TIME TO RTC");
        ApplyRTCTime();
        break;
    }
    int tt[3] = {10, 10, 10};
    int dp[6] = {3, 3, 3, 3, 3, 3};
    SetNixieSchematicByTime(tt, dp, true);
}

void ReadCurrentTime()
{
    for (int cnt = 0; cnt <= 6; cnt++)
    {
        if (cnt == 3)
            continue;
        if (time[cnt] == 0 | cnt <= 2)
            time[cnt] = rtc.GetTime(cnt);
    }
    return;
}

void ApplyRTCTime()
{
    rtc.SetDateTime(time[YEAR], time[MONTH], time[DAY], time[HOUR], time[MINUTE], time[SECOND]);
}

void SetNixieSchematicByTime(int *code, int *dpCode = 0, bool none = false)
{
    for (int cnt = 0; cnt < 3; cnt++)
    {
        nixie.schematic[2 * cnt][0] = none == false ? code[3 - cnt] / 10 : 10;
        nixie.schematic[2 * cnt][1] = dpCode[2 * cnt];
        nixie.schematic[2 * cnt + 1][0] = none == false ? code[3 - cnt] % 10 : 10;
        nixie.schematic[2 * cnt + 1][1] = dpCode[2 * cnt + 1];
    }
}

void LimitDateValue(int parameter, int max, int min)
{
    if (time[parameter] > max)
        time[parameter] = min;
    if (time[parameter] < min)
        time[parameter] = max;
}