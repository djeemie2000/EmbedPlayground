#ifndef ARENVELOPE_H
#define ARENVELOPE_H

template<class T>
class CAREnvelope
{
public:
    CAREnvelope()
        : m_Envelope(0)
        , m_Increment(0)
        , m_AttackIncrement(1)
        , m_ReleaseIncrement(-1)
    {}

    void SetAttackSamples(T Attack)
    {
        if(0<Attack)
        {
            m_AttackIncrement = 1/Attack;
        }
        else
        {
            m_AttackIncrement = 1;
        }
    }

    void SetReleaseSamples(T Release)
    {
        if(0<Release)
        {
            m_ReleaseIncrement = -1/Release;
        }
        else
        {
            m_ReleaseIncrement = -1;
        }
    }

    void NoteOn()
    {
        m_Envelope = 0;
        m_Increment = m_AttackIncrement;
    }

    void NoteOff()
    {
        m_Increment = m_ReleaseIncrement;
    }


    T operator()()
    {
        if(m_Increment)
        {
            m_Envelope += m_Increment;

            if(0<m_Increment)
            { // positive increment
                if(1<m_Envelope)
                {
                    // goto sustain phase at 1
                    m_Increment = 0;
                    m_Envelope = 1;
                }
            }
            else
            { // negative increment
                if(m_Envelope<0)
                {
                    m_Increment = 0;
                    m_Envelope = 0;
                }
            }
        }

        return m_Envelope;
    }

private:
    T m_Envelope;
    T m_Increment;
    T m_AttackIncrement;
    T m_ReleaseIncrement;
};

#endif // ARENVELOPE_H
