#pragma once

#include "Oscillator.h"


template<class T>
class COscillatorSource
{
public:
    COscillatorSource(T SamplingFrequency)
     : m_Oscillator(SamplingFrequency)
     , m_FrequencyHz(110)
     , m_Amplitude(0)
    {}

    T Render()
    {
        return m_Oscillator(m_FrequencyHz, m_Amplitude, m_DetuneA, m_DetuneB);
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
        m_DetuneA = Detune;
        m_DetuneB = 1/Detune;
    }

private:
    COscillator<T> m_Oscillator;

public:
    T m_FrequencyHz;
    T m_Amplitude;
    T m_DetuneA;
    T m_DetuneB;
};
