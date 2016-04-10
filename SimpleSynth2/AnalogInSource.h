#pragma once

#include "mbed.h"


template<class T>
class CAnalogInSource
{
public:
    CAnalogInSource(PinName Pin)
     : m_In(Pin)
     , m_Amplitude(1)
    {}

    T Render()
    {
        return m_Amplitude*(2*m_In.read()-1);
    }

    void SetAmplitude(T Amplitude)
    {
        m_Amplitude = Amplitude;
    }

private:
    AnalogIn m_In;

    T m_Amplitude;
};
