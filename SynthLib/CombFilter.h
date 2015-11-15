#pragma once

#include "DelayLine2.h"

template<class T, int SamplingFrequency, int MinFrequency>
class CCombFilter
{
public:
    CCombFilter()
     : m_SamplingFrequency(SamplingFrequency)
     , m_MinFrequency(MinFrequency)
     , m_DelayLine(0)
    {
    }

    T operator()(T In, T Feedback, T Frequency)
    {
        // assumes Freq > 0
        T Delay = m_MinFrequency<Frequency ?  m_SamplingFrequency/Frequency : m_SamplingFrequency/m_MinFrequency;
        T DelayValue = m_DelayLine.Read(Delay);
        m_DelayLine.Write(In + Feedback * DelayValue);
        return DelayValue;
    }

private:
    const T m_SamplingFrequency;
    const T m_MinFrequency;
    CDelayLine2<T, SamplingFrequency/MinFrequency> m_DelayLine;
};
