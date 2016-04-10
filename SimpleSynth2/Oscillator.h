#pragma once

#include "PhaseStep.h"
#include "PhaseAccumulator.h"
#include "Pulse.h"


template<class T>
class COscillator
{
public:
    COscillator(T SamplingFrequency)
     : m_PhaseStepA(SamplingFrequency)
     , m_PhaseAccumulatorA()
     , m_DetuneA(1.01f)
     , m_PhaseStepB(SamplingFrequency)
     , m_PhaseAccumulatorB()
     , m_DetuneB(1/m_DetuneA)
     , m_PhaseStepSub(SamplingFrequency)
     , m_PhaseAccumulatorSub()
     , m_DetuneSub(0.5f)
     , m_OscillatorSub()
    {}

    int operator()(T FrequencyHz, T Amplitude, T DetuneA, T DetuneB)
    {
        T Value = m_PhaseAccumulatorA(m_PhaseStepA(FrequencyHz*DetuneA))
                + m_PhaseAccumulatorB(m_PhaseStepB(FrequencyHz*DetuneB))
                + m_OscillatorSub(m_PhaseAccumulatorSub(m_PhaseStepSub(FrequencyHz*m_DetuneSub)));

        return 0.33f*Value;
        //return (1<<15)*(1+Amplitude*0.33f*Value);
    }

private:
    CPhaseStep<T> m_PhaseStepA;
    CPhaseAccumulator<T> m_PhaseAccumulatorA;
    T m_DetuneA;

    CPhaseStep<T> m_PhaseStepB;
    CPhaseAccumulator<T> m_PhaseAccumulatorB;
    T m_DetuneB;

    CPhaseStep<T> m_PhaseStepSub;
    CPhaseAccumulator<T> m_PhaseAccumulatorSub;
    const T m_DetuneSub;
    CPulsePos<T> m_OscillatorSub;
};
