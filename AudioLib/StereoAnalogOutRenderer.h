#pragma once

#include "AnalogOut.h"
#include "PinNames.h"
#include "StereoValue.h"

template<class T>
class CStereoAnalogOutRenderer
{
public:
    CStereoAnalogOutRenderer(PinName AnalogOutPinLeft, PinName AnalogOutPinRight)
     : m_AnalogOutLeft(AnalogOutPinLeft)
     , m_AnalogOutRight(AnalogOutPinRight)
    {}

    void Render(const CStereoValue<T>& Value)
    {
        m_AnalogOutLeft.write( (1+Value.m_Left)/2 );//[-1,+1] to [0,1]
        m_AnalogOutRight.write( (1+Value.m_Right)/2 );//[-1,+1] to [0,1]
    }

private:
    AnalogOut m_AnalogOutLeft;
    AnalogOut m_AnalogOutRight;
};
