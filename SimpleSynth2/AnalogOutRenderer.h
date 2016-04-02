#pragma once

#include "AnalogOut.h"

template<class T>
class CAnalogOutRenderer
{
public:
    CAnalogOutRenderer(PinName AnalogOutPin)
     : m_AnalogOut(AnalogOutPin)
    {}

    void Render(T Value)
    {
        m_AnalogOut.write_u16(Value);
    }

private:
    AnalogOut m_AnalogOut;
};
