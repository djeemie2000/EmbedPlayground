#pragma once

#include "Noise.h"
#include "DelayLine2.h"
#include "FourPoleFilter.h"
#include "DeltaSmooth.h"
#include "AREnvelope.h"
#include "ConstNumSamplesGenerator.h"

namespace synthlib
{

//TODO
//capacity of delayline determined by lowest frequency
// ...

template<int Size>
class CSelector
{
public:
    CSelector()
     : m_Current(-1)
     , m_Active()
    {
        for(int idx = 0; idx<Size; ++idx)
        {
            m_Active[idx] = true;
        }
    }

    int Select()
    {
        int Selected = -1;
        for(int idx = 1; idx<Size && Selected==-1; ++idx)
        {
            int Tmp = (m_Current+idx)%Size;
            if(m_Active[Tmp])
            {
                Selected = Tmp;
                m_Current = Tmp;
            }
        }
        return Selected;
    }

    bool GetActive(int Idx) const
    {
        return m_Active[Idx];
    }

    void SetActive(int Idx, bool Active)
    {
        m_Active[Idx] = Active;
    }

private:
    int m_Current;
    bool m_Active[Size];
};



template<class T, int Capacity, int NumOperators>
class CPolyKarplusStrong
{
public:

    CPolyKarplusStrong()
        //: m_MinFrequency(MinFrequency)
        : m_SamplingFrequencyHz(40000)
        , m_ExciterNoise()
        , m_ExciterLPF()
        , m_Operator()
    {
    }

    void SetSamplingFrequency(std::uint64_t SamplingFrequencyHz)
    {
        m_SamplingFrequencyHz = SamplingFrequencyHz;
    }

    void Excite(int Operator, T Excitation, T Frequency, T Damp, T AttackMilliSeconds, T Pan)
    {
        if(0<=Operator && Operator<NumOperators)
        {
            m_ExciterLPF.SetParameter(Excitation);
            T Period = m_SamplingFrequencyHz/Frequency;
            if(Period<Capacity)
            {
                T AttackSamples = CConstNumSamplesGenerator<T>(m_SamplingFrequencyHz).SetMilliSeconds(AttackMilliSeconds);

                m_Operator[Operator].m_GainLeft = std::min(T(1), 1-Pan);
                m_Operator[Operator].m_GainRight = std::min(T(1), 1+Pan);

                m_Operator[Operator].ExciteOperator(Damp, Period, AttackSamples);
            }
        }
    }

    T operator()()
    {
        T Excite = m_ExciterLPF(m_ExciterNoise());

        T Out = 0;
        for(int idx = 0; idx<NumOperators; ++idx)
        {
            Out += m_Operator[idx](Excite);
        }

        return Out;//no normalisation
    }

    void operator()(T& Left, T& Right)
    {
        Left = 0;
        Right = 0;

        T Excite = m_ExciterLPF(m_ExciterNoise());

        for(int idx = 0; idx<NumOperators; ++idx)
        {
            T Out = m_Operator[idx](Excite);
            Left += m_Operator[idx].m_GainLeft*Out;
            Right += m_Operator[idx].m_GainRight*Out;
        }
    }

private:
    struct SOperator
    {
        SOperator()
            : m_Period(1)
            , m_Cntr(0)// not excited
            , m_DelayLine(0)
            , m_DampLPF()
            , m_DCOffset(0, T(1)/8192)
            , m_Envelope()
            , m_GainLeft(1)
            , m_GainRight(1)
        {}

        void ExciteOperator(T Damp, int Period, T AttackSamples)
        {
            m_DampLPF.SetParameter(Damp);
            m_DCOffset.Reset(0);
            m_Envelope.SetAttackSamples(AttackSamples);
            m_Envelope.NoteOn();
            m_Period = Period;
            m_Cntr = m_Period;
        }

        T operator()(T Excite)
        {
            T WriteValue = 0<m_Cntr ? Excite : m_DampLPF(m_DelayLine.Read(m_Period));
            m_DelayLine.Write(WriteValue);
            if(m_Cntr)
            {
                --m_Cntr;
            }

            return m_Envelope()*(WriteValue-m_DCOffset(WriteValue));//m_Envelope(WriteValue-m_DCOffset(WriteValue));//
        }

        int m_Period;
        int m_Cntr;
        CDelayLine2<T, Capacity> m_DelayLine;//used as circular buffer
        CFourPoleLowPassFilter<T> m_DampLPF;
        synthlib::CDeltaSmooth<T> m_DCOffset;
        CAREnvelope<T> m_Envelope;
        T m_GainLeft;
        T m_GainRight;
    };

    //const T m_MinFrequency;
    std::uint64_t m_SamplingFrequencyHz;

    CNoise<T> m_ExciterNoise;
    CFourPoleLowPassFilter<T> m_ExciterLPF;

    SOperator m_Operator[NumOperators];
};

}
