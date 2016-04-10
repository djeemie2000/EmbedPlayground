#pragma once

#include "mbed.h"


template<class T>
class CAnalogInSource
{
public:
    CAnalogInSource(PinName Pin)
     : m_In(Pin)
     , m_Amplitude(1)
     , m_Min(1)
     , m_Max(-1)
    {}

    T Render()
    {
        T Value = m_Amplitude*(2*m_In.read()-1);

        m_Min = Value<m_Min ? Value : m_Min;
        m_Max = m_Max<Value ? Value : m_Max;

        return Value;
    }

    void SetAmplitude(T Amplitude)
    {
        m_Amplitude = Amplitude;
    }

    void ResetMinMax()
    {
        m_Min = 1;
        m_Max = -1;
    }

private:
    AnalogIn m_In;

    T m_Amplitude;

public:
    T m_Min;
    T m_Max;
};
