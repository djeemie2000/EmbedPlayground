#pragma once

namespace synthlib
{

template<class T>
class CDeltaSmooth
{
public:
    CDeltaSmooth(T Initial, T Delta)
        : m_Delta(Delta)
        , m_Value(Initial)
    {}

    void Reset(T Value)
    {
        m_Value = Value;
    }

    T operator()(T In)
    {
        if(m_Value<In)
        {
            m_Value += m_Delta;
        }
        else if(In<m_Value)
        {
            m_Value -= m_Delta;
        }

        return m_Value;
    }

private:
    const T m_Delta;
    T m_Value;
};

}
