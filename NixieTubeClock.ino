
#include "DS3234.h"
#include "Nixie.h"
//PIN ASSIGN

#define SW_ACCEPT 20
#define SW_BACK 21
#define SW_N 19
#define SW_P 18
//MODE SETTINGS
#define MODE_SERIAL 2
#define MODE_SETTING 1
#define MODE_DISPLAY 0

//fLASHING SETTINGS
#define FLASHING_PERIOD 250 //milli seconds

#define KEY_FLAG_STATE 0 // 0=pressed 1=pressing

DS3234 rtc;
Nixie nixie;
unsigned long elaped_time = 0;
unsigned long current_time = 0;

int time[7] = {0, 0, 0, 0, 0, 0};
int _mode = 0;
int setting_index = 6;
int flashing_state = 0;
int serialIndex = 6;

int pre_sw_accept = 0;
int sw_accept_flag = 0;
int pre_sw_back = 0;
int sw_back_flag = 0;
int pre_sw_n = 0;
int sw_n_flag = 0;
int pre_sw_p = 0;
int sw_p_flag = 0;

void ReadCurrentTime();
void SetUpTimeBySerial(String str);
void SetNixieSchematicByTime(bool isDateCode = false, bool flashing = false, int flashing_index = 0);
void Reflesh_Display();
void SetUpTime();
void ReadKeyState();
void LimitDateValue(int parameter, int max, int min);
void ResetFlag()
{
    sw_accept_flag = 0;
    sw_back_flag = 0;
    sw_n_flag = 0;
    sw_p_flag = 0;
    return;
}

void setup()
{
    //バグが認めらるので一時退避
    //  MsTimer2::set(20, Reflesh_Display);
    //  MsTimer2::start();

    //Switch Initialize
    for (int i = 18; i <= 21; i++)
        pinMode(i, INPUT);

    Serial.begin(9600);
}

void loop()
{
    ReadKeyState();
    Reflesh_Display();

    if (Serial.available() > 0)
    {
        String str = Serial.readString();
        Serial.println(str);
        if (_mode == MODE_SERIAL)
        {
            SetUpTimeBySerial(str);
        }
        if (str.equals("SET"))
        {
            Serial.println("mode is SERIAL");
            _mode = MODE_SERIAL;
        }
        if (str.equals("QUIT"))
        {
            Serial.println("mode is DISPLAY");
            _mode = MODE_DISPLAY;
            serialIndex = 6;
        }
        return;
    }
    if (_mode == MODE_SERIAL)
    {
        for (int cnt = 0; cnt < 3; cnt++)
        {
            nixie.schematic[2 * cnt][0] = 10;
            nixie.schematic[2 * cnt][1] = 3;
            nixie.schematic[2 * cnt + 1][0] = 10;
            nixie.schematic[2 * cnt + 1][1] = 3;
        }
        return;
    }
    //current_time = millis();
    if (_mode == MODE_SETTING)
    {
        //Serial.println("SETUP MODE");
        SetUpTime();
        ResetFlag();
        return;
    }

    if (sw_back_flag == 1)
    {
        //Serial.println("ENTER SETUP MODE");
        _mode = MODE_SETTING;
        ResetFlag();
        return;
    }

    ResetFlag();
    //Serial.println("Read Current Time");

    ReadCurrentTime();
    SetNixieSchematicByTime();
}

void ReadCurrentTime()
{
    int result[7];
    for (int cnt = 0; cnt <= 6; cnt++)
    {
        if (cnt == 3)
            continue;
        time[cnt] = rtc.GetTime(cnt);
    }
    return;
}
void SetUpTimeBySerial(String str)
{
    //Serial.println(serialIndex);
    Serial.println(str);
    time[serialIndex] = str.toInt();
    Serial.print(serialIndex);
    Serial.print(" was set to ");
    Serial.println(time[serialIndex]);
    if (serialIndex == 0)
    {
        rtc.SetDateTime(time[YEAR], time[MONTH], time[DAY], time[HOUR], time[MINUTE], time[SECOND]);
        Serial.println("Time is Renewed");
        _mode = MODE_DISPLAY;
        serialIndex = 6;
        return;
    }
    serialIndex--;

    for (int cnt = 0; cnt < 3; cnt++)
    {
        nixie.schematic[2 * cnt][0] = 10;
        nixie.schematic[2 * cnt + 1][0] = 10;
        nixie.schematic[2 * cnt + 1][1] = 3;
    }
}

void SetNixieSchematicByTime(bool isDateCode = false, bool flashing = false, int flashing_index = 0)
{
    int code = isDateCode == true ? 6 : 2;

    for (int cnt = 0; cnt < 3; cnt++)
    {
        nixie.schematic[2 * cnt][0] = time[code - cnt] / 10;
        nixie.schematic[2 * cnt][1] = 0;
        nixie.schematic[2 * cnt + 1][0] = time[code - cnt] % 10;
        nixie.schematic[2 * cnt + 1][1] = 2;
    }
}

void Reflesh_Display()
{
    nixie.ShowDisplay();
}

void SetUpTime()
{
    //アクセプトスイッチ(SW2)印可時の処理
    if (sw_accept_flag == 1)
    {
        //設定モード抜け
        if (setting_index == 0)
        {
            setting_index = 6;
            _mode = MODE_DISPLAY;
            rtc.SetDateTime(time[YEAR], time[MONTH], time[DAY], time[HOUR], time[MINUTE], time[SECOND]);
            //SetDateTime();
            return;
        }
        //設定部分切り替え
        setting_index--;
        if (setting_index == 3)
            setting_index--;
    }

    //バックスイッチ(SW3)印可時の処理
    if (sw_back_flag == 1)
    {
        //setting_index = setting_index >= 6 ? setting_index : setting_index++;
        if (setting_index >= 6)
            setting_index = 6;
        else
            setting_index++;

        if (setting_index == 3)
            setting_index++;
    }

    //＋スイッチ(SW4)印可時の処理
    if (sw_p_flag == 1)
    {
        time[setting_index]++;
    }

    //-スイッチ印可時の処理
    if (sw_n_flag == 1)
    {
        time[setting_index]--;
    }

    //最大値を設定
    LimitDateValue(SECOND, 59, 1);
    LimitDateValue(MINUTE, 59, 1);
    LimitDateValue(HOUR, 23, 0);
    LimitDateValue(DAY, 31, 1);
    LimitDateValue(MONTH, 12, 1);
    LimitDateValue(YEAR, 99, 0);

    //以下表示用
    if (setting_index >= 4)
    {
        //year code
        SetNixieSchematicByTime(true, true, setting_index - 4);
        return;
    }
    SetNixieSchematicByTime(false, true, setting_index);
    return;
}

void ReadKeyState()
{
    int sw_accept = digitalRead(A2); //SW_ACCEPT);
    if (sw_accept != pre_sw_accept && sw_accept == KEY_FLAG_STATE)
        sw_accept_flag = 1;

    int sw_back = digitalRead(A3); //SW_BACK);
    if (sw_back != pre_sw_back && sw_back == KEY_FLAG_STATE)
        sw_back_flag = 1;

    int sw_n = digitalRead(A1); //SW_N);
    if (sw_n != pre_sw_n && sw_n == KEY_FLAG_STATE)
        sw_n_flag = 1;

    int sw_p = digitalRead(A0); //SW_P);
    if (sw_p != pre_sw_p && sw_p == KEY_FLAG_STATE)
        sw_p_flag = 1;

    pre_sw_accept = sw_accept;
    pre_sw_back = sw_back;
    pre_sw_n = sw_n;
    pre_sw_p = sw_p;
}

void LimitDateValue(int parameter, int max, int min)
{
    if (time[parameter] > max)
        time[parameter] = min;
    if (time[parameter] < min)
        time[parameter] = max;
}