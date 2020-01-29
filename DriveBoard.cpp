#include "DriveBoard.h"

uint8_t DriveBoard::ReadKeyState()
{
    for (int cnt = 18; cnt <= 21; cnt++)
    {
        int state = digitalRead(cnt);
        if (bitRead(keyState, cnt - 18) != state)
        {
            keyState = state << (cnt - 14);
        }
        keyState += digitalRead(cnt) << (cnt - 18);
    }
    return keyState;
}

DriveBoard::DriveBoard()
{
    for (int cnt = 18; cnt <= 21; cnt++)
        pinMode(cnt, INPUT);
}

DriveBoard::~DriveBoard()
{
}
