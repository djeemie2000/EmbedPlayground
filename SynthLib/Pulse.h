#ifndef Pulse_H
#define Pulse_H

template<class T>
class CPulsePos
{
public:
    CPulsePos(){}

    T operator()(T Phase) const
    {
        // square wave with positive part first
        return Phase<0 ? 1 : -1;
    }
};

template<class T>
class CPulseNeg
{
public:
    CPulseNeg(){}

    T operator()(T Phase) const
    {
        // square wave with negative part first
        return Phase<0 ? -1 : 1;
    }
};

#endif // Pulse_H
