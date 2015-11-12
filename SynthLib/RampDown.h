#ifndef RAMPDOWN_H
#define RAMPDOWN_H

template<class T>
class CRampDown
{
public:
    T operator()(const T& Phase) const
    {
        //  [-1,+1] -> [+1,-1]
        return -Phase;
    }
};

#endif // RAMPDOWN_H
