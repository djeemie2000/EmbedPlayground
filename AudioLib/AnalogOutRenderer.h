#pragma once

#include "AnalogOut.h"
#include "mbed.h"

template<class T>
class CAnalogOutRenderer
{
public:
    CAnalogOutRenderer(PinName AnalogOutPin)
     : m_AnalogOut(AnalogOutPin)
    {}

    void Render(T Value)
    {
        m_AnalogOut.write( (1+Value)/2 );//[-1,+1] to [0,1]
    }

private:
    AnalogOut m_AnalogOut;
};
