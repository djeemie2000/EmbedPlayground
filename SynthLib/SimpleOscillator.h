#ifndef SIMPLEOSCILLATOR_H
#define SIMPLEOSCILLATOR_H

#include "PhaseStep.h"
#include "PhaseAccumulator.h"
#include "SelectableOperator.h"
#include "SelectableOperatorFactory.h"

template<class T>
class CSimpleOscillator
{
public:
    CSimpleOscillator(T SamplingFrequency)
     : m_PhaseStep(SamplingFrequency)
     , m_PhaseGen()
     , m_Operator(CSelectableOperatorFactory::Create())
     , m_PhaseStepSign(1)
    {
        m_Operator.Select(0);
        m_PhaseStep.SetFrequency(440.0f);
    }

    void Sync()
    {
        m_PhaseGen.Sync();
    }

    void SoftSync()
    {
        m_PhaseStepSign = -m_PhaseStepSign;
    }

    void SetFrequency(T Frequency)
    {
        m_PhaseStep.SetFrequency(Frequency);
    }

    void SelectWaveform(int Selection)
    {
        m_Operator.Select(Selection);
    }

    T operator()(T Frequency)
    {
        return m_Operator(m_PhaseGen(m_PhaseStepSign*m_PhaseStep(Frequency)));
    }

    T operator()()
    {
        return m_Operator(m_PhaseGen(m_PhaseStepSign*m_PhaseStep()));
    }

private:
    CPhaseStep<T> m_PhaseStep;
    CPhaseAccumulator<T> m_PhaseGen;
    CSelectableOperator<T> m_Operator;
    T m_PhaseStepSign;
};

#endif // SIMPLEOSCILLATOR_H
