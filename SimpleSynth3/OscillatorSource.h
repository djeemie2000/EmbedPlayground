#pragma once

#include "mbed.h"
#include "PolyKarplusStrong.h"
#include "FourPoleFilter.h"


template<class T, int Capacity>
class COscillatorSource
{
public:
    COscillatorSource(T SamplingFrequency)
     : m_Oscillator()
     , m_FrequencyHz(110)
     , m_Amplitude(0)
     , m_Trigger(false)
     , m_Damp(1)
     , m_Excite(1)
     , m_Attack(0)
     , m_Counter(0)
     , m_Gate(D7)
     , m_PitchCVIn(A4)
     , m_PitchCV(0)
     , m_DampCV(A0)
     , m_ExciteCV(A1)
     , m_AttackCV(A3)
    {
        m_Oscillator.SetSamplingFrequency(SamplingFrequency);
    }

    T Render()
    {
        ReadControls();

        if(m_Trigger)
        {
            m_Oscillator.Excite(m_Selector.Select(), m_Excite, m_FrequencyHz, m_Damp, m_Attack*1000, 0);
        }

        return m_Oscillator();
    }

    void SetAmplitude(int Amplitude)
    {
        m_Trigger = !m_Amplitude && Amplitude;
        m_Amplitude = Amplitude;
    }

    void SetFrequencyHz(T FrequencyHz)
    {
        m_FrequencyHz = FrequencyHz;
    }

    void SetDamp(T Damp)
    {
        m_Damp = Damp;
    }

    void SetExcite(T CutOff)
    {
        m_Excite = (1+3*CutOff)/4;// [0,1] -> [0.25, 1]
    }

    void SetAttack(T Attack)
    {
        m_Attack = Attack;
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
            SetDamp(m_DampCV.read());
        }
        else if(m_Counter==3)
        {
            SetExcite(m_ExciteCV.read());
        }
        else if(m_Counter==4)
        {
            SetAttack(m_AttackCV.read());
        }

        ++m_Counter;
        if(5<=m_Counter)
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
    static const int NumOperators = 8;
    synthlib::CSelector<NumOperators>                       m_Selector;
    synthlib::CPolyKarplusStrong<T, Capacity, NumOperators> m_Oscillator;

public:
    T m_FrequencyHz;
    int m_Amplitude;
    bool m_Trigger;
    T m_Damp;
    T m_Excite;
    T m_Attack;

private:
    int m_Counter;
    DigitalIn m_Gate;
    AnalogIn m_PitchCVIn;
    float m_PitchCV;
    AnalogIn m_DampCV;
    AnalogIn m_ExciteCV;
    AnalogIn m_AttackCV;
};
