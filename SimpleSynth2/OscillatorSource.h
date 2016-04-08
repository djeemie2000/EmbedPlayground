#pragma once

#include "Oscillator.h"
#include "FourPoleFilter.h"


template<class T>
class COscillatorSource
{
public:
    COscillatorSource(T SamplingFrequency)
     : m_Oscillator(SamplingFrequency)
     , m_FrequencyHz(110)
     , m_Amplitude(0)
     , m_DetuneA(1)
     , m_DetuneB(1)
     , m_CutOff(1)
    {}

    T Render()
    {
        return m_LPF(m_Oscillator(m_FrequencyHz, m_Amplitude, m_DetuneA, m_DetuneB), m_CutOff);
    }

    void SetAmplitude(T Amplitude)
    {
        m_Amplitude = Amplitude;
    }

    void SetFrequencyHz(T FrequencyHz)
    {
        m_FrequencyHz = FrequencyHz;
    }

    void SetDetune(T Detune)
    {
        m_DetuneA = 1+Detune/2;
        m_DetuneB = 1/m_DetuneA;
        //m_DetuneB = 1-Detune/2;
    }

    void SetCutoff(T CutOff)
    {
        m_CutOff = (1+3*CutOff)/4;// [0,1] -> [0.25, 1]
    }

private:
    COscillator<T> m_Oscillator;
    CFourPoleLowPassFilter<T> m_LPF;

public:
    T m_FrequencyHz;
    T m_Amplitude;
    T m_DetuneA;
    T m_DetuneB;
    T m_CutOff;
};
