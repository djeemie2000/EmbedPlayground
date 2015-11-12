#ifndef RAMPUPDOWN_H
#define RAMPUPDOWN_H

template<class T>
class CRampUpDown
{
public:
    CRampUpDown()
    {}

    T operator()(T Phase) const
    {
        return Phase<0 ? Phase : 1-Phase;
    }
};

#endif // RAMPUPDOWN_H
