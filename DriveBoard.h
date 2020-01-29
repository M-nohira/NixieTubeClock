#include <Arduino.h>

#define SW_ACCEPT 20
#define SW_BACK 21
#define SW_N 19
#define SW_P 18

class DriveBoard
{
private:
    uint8_t keyState = 0;
public:
    uint8_t ReadKeyState();
    
    DriveBoard(/* args */);
    ~DriveBoard();
};
