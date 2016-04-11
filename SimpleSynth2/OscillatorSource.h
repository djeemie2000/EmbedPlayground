#pragma once

#include "mbed.h"
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
     , m_Counter(0)
     , m_Gate(D7)
     , m_PitchCVIn(A4)
     , m_PitchCV(0)
     , m_DetuneCV(A0)
     , m_CutoffCV(A1)

    {}

    T Render()
    {
        ReadControls();

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

    void ReadControls()
    {
        // alternate so only 1 control is read every render()
        if(m_Counter==0)
        {
            SetAmplitude(m_Gate.read());
        }
        else if(m_Counter==1)
        {
            SamplePitch();
        }
        else if(m_Counter==2)
        {
            SetDetune(m_DetuneCV.read());
        }
        else if(m_Counter==3)
        {
            SetCutoff(m_CutoffCV.read());
        }

        ++m_Counter;
        if(4<=m_Counter)
        {
            m_Counter = 0;
        }
     }

     void SamplePitch()
     {
         // TODO try a one pole LPF on Value
         m_PitchCV = (m_PitchCVIn.read()+m_PitchCV)/2;

         float ReferenceVltage = 3.3f;
         float VltageMult = 2.03f;//????
         float Vltage = VltageMult*m_PitchCV*ReferenceVltage;
         int MidiNote = Vltage*12 + 0.5f;

         float Freq = GetMidiNoteFrequencyMilliHz(MidiNote+24)/1000.0f;//starts with C1?
         SetFrequencyHz(Freq);
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

private:
    int m_Counter;
    DigitalIn m_Gate;
    AnalogIn m_PitchCVIn;
    float m_PitchCV;
    AnalogIn m_DetuneCV;
    AnalogIn m_CutoffCV;
};
