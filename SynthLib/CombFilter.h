#pragma once

#include "DelayLine2.h"

template<class T>
class CCombFilter
{
public:
    CCombFilter(T SamplingFrequency, T MinFrequency)
     : m_SamplingFrequency(SamplingFrequency)
     , m_MinFrequency(MinFrequency)
     , m_DelayLine(SamplingFrequency/MinFrequency, 0)
    {
    }

    T operator()(T In, T Feedback, T Frequency)
    {
        // assumes Freq > 0
        T Delay = m_MinFrequency<Frequency ?  m_SamplingFrequency/Frequency : m_SamplingFrequency/m_MinFrequency;
        //Delay = Delay<m_DelayLine.GetCapacity() ? Delay : m_DelayLine.GetCapacity();
        T DelayValue = m_DelayLine.Read(Delay);
        m_DelayLine.Write(In + Feedback * DelayValue);
        return DelayValue;
    }

private:
    const T m_SamplingFrequency;
    const T m_MinFrequency;
    CDelayLine2<T> m_DelayLine;
};
