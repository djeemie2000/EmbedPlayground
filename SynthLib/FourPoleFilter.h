#pragma once

#include <cstdlib>
#include <array>

template<class T>
class CFourPoleLowPassFilter
{
public:
    CFourPoleLowPassFilter()
        : m_OutMin1(0)
        , m_OutMin2(0)
        , m_OutMin3(0)
        , m_OutMin4(0)
        , m_A0(1)
        , m_B1(0)
        , m_B2(0)
        , m_B3(0)
        , m_B4(0)
    {}

    void SetParameter(T Parameter)
    {
        // parameter in [0,1] range
        // parameter is 1 => no filtering
        // parameter is 0 => lowest cutoff
        T Param = 1-Parameter*Parameter;
        m_A0 = (1-Param)*(1-Param)*(1-Param)*(1-Param);
        m_B1 = 4*Param;
        m_B2 = -6*Param*Param;
        m_B3 = 4*Param*Param*Param;
        m_B4 = -Param*Param*Param*Param;
    }

    T operator()(T In)
    {
        T Out = m_A0*In + m_B1*m_OutMin1 + m_B2*m_OutMin2 + m_B3*m_OutMin3 + m_B4*m_OutMin4;
        m_OutMin4 = m_OutMin3;
        m_OutMin3 = m_OutMin2;
        m_OutMin2 = m_OutMin1;
        m_OutMin1 = Out;
        return Out;
    }

    T operator()(T In, T Parameter)
    {
        SetParameter(Parameter);
        return this->operator ()(In);//??syntax??
    }

private:
    T m_OutMin1;
    T m_OutMin2;
    T m_OutMin3;
    T m_OutMin4;
    T m_A0;
    T m_B1;
    T m_B2;
    T m_B3;
    T m_B4;
};
