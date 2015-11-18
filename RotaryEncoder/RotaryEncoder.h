#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include "mbed.h"

class CRotaryEncoder
{
public:
    CRotaryEncoder();

    void Begin(PinName PinA, PinName PinB);
    void Read();
    int GetPosition() const;
    void Reset();//!< reset to position, change count, ...

    int GetChangeCount() const;
    unsigned int GetRotaryHistory() const;

private:
    gpio_t m_PinA;
    gpio_t m_PinB;
    unsigned int m_RotaryHistory;
    int m_Position;
    int m_ChangeCount;
};

#endif // ROTARYENCODER_H
