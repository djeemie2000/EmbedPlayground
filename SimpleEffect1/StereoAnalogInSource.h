#pragma once

#include "mbed.h"
#include "FourPoleFilter.h"
#include "StereoValue.h"

template<class T>
class CStereoAnalogInSource
{
public:
    CStereoAnalogInSource(PinName Pin)
     : m_In(Pin)
     , m_Amplitude(1)
     , m_CutoffCV(A1)
     , m_CutOff(1)
    {}

    CStereoValue<T> Render()
    {
        ReadControls();

        T InValue = 2*m_In.read()-1;
        T Value = m_Amplitude*m_LPF(InValue, m_CutOff);
        return { Value, m_Amplitude*InValue };
    }

    void SetAmplitude(T Amplitude)
    {
        m_Amplitude = Amplitude;
    }

    void SetCutoff(T CutOff)
    {
        m_CutOff = (1+3*CutOff)/4;// [0,1] -> [0.25, 1]
    }

    void ReadControls()
    {
        // alternate so only 1 control is read every render()
        if(m_Counter==0)
        {
            SetCutoff(m_CutoffCV.read());
        }

        ++m_Counter;
        if(1<=m_Counter)
        {
            m_Counter = 0;
        }
     }


private:
    AnalogIn m_In;
    T m_Amplitude;
    CFourPoleLowPassFilter<T> m_LPF;

    int m_Counter;
//    DigitalIn m_Gate;
    AnalogIn m_CutoffCV;

public:
    T m_CutOff;
};
