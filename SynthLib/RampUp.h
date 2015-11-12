#ifndef RAMPUP_H
#define RAMPUP_H

template<class T>
class CRampUp
{
public:
    T operator()(const T& Phase) const
    {
        return Phase;
    }
};

#endif // RAMPUP_H
