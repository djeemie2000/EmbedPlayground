#ifndef DELAYLINE2_H
#define DELAYLINE2_H

#include "algorithm"

template<class T, int Capacity>
class CDelayLine2
{
public:
    CDelayLine2(T InitialValue)
     : m_Values()
     , m_WriteIndex(0)
    {
        std::fill(m_Values, m_Values+Capacity, InitialValue);
    }

    int GetCapacity() const
    {
        return Capacity;
    }

    void Write(T In)
    {
        ++m_WriteIndex;
        if(Capacity<=m_WriteIndex)
        {
            m_WriteIndex = 0;
        }
        m_Values[m_WriteIndex] = In;
    }

    T Read(int Delay)
    {
        int Idx = m_WriteIndex-Delay;
        return m_Values[0<=Idx ? Idx : Idx+Capacity];
    }

private:
    T m_Values[Capacity];
    int m_WriteIndex;
};

#endif // DELAYLINE2_H
