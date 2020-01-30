#include "DriveBoard.h"

uint8_t DriveBoard::ReadKeyState()
{
    uint8_t c_keyState = 0;
    for (int cnt = 14; cnt <= 17; cnt++)
    {
        int state = digitalRead(cnt);

        if (bitRead(keyState, cnt - 14) != state)
        {
            c_keyState += state << (cnt - 10);
        }
        c_keyState += state << (cnt - 14);
    }

    keyState = c_keyState;
    return c_keyState;
}

DriveBoard::DriveBoard()
{
    for (int cnt = 14; cnt <= 17; cnt++)
        pinMode(cnt, INPUT);
}

DriveBoard::~DriveBoard()
{
}
