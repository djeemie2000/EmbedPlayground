#pragma once

template<class T>
class CPhaseStep
{
public:
    CPhaseStep(const T& SamplingFrequency)
        : m_SamplingFrequency(SamplingFrequency)
        , m_PhaseStep(0)
        , m_TwoDivSamplingFrequency(2/SamplingFrequency)
    {}

    CPhaseStep()
        : m_SamplingFrequency(1)
        , m_PhaseStep(0)
    {}

    void SetFrequency(T Frequency)
    {
        // cycle through [-1,+1] in 1 period
        m_PhaseStep = 2*Frequency/m_SamplingFrequency;
    }

    T operator()()
    {
        return m_PhaseStep;
    }

    T operator()(T Frequency)
    {
        //return 2*Frequency/m_SamplingFrequency;
        return m_TwoDivSamplingFrequency*Frequency;
    }

private:
    T m_SamplingFrequency;
    T m_PhaseStep;
    const T m_TwoDivSamplingFrequency;
};
