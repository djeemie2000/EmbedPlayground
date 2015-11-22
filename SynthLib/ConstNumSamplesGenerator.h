#ifndef CONSTNUMSAMPLESGENERATOR_H
#define CONSTNUMSAMPLESGENERATOR_H

template<class T>
class CConstNumSamplesGenerator
{
public:
    CConstNumSamplesGenerator(T SamplingFrequency)
        : m_SamplingFrequency(SamplingFrequency)
        , m_Value(0)
    {}

    T SetMilliSeconds(T MilliSeconds)
    {
        m_Value = MilliSeconds*m_SamplingFrequency/1000;
        return m_Value;
    }

    T operator()()
    {
        return m_Value;
    }

private:
    T m_SamplingFrequency;
    T m_Value;
};

#endif // CONSTNUMSAMPLESGENERATOR_H
