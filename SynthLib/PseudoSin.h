#ifndef PSEUDOSIN_H
#define PSEUDOSIN_H

template<class T>
class CPseudoSin
{
public:
    T operator()(const T& Phase) const
    {
        //T Phase01 = (1+Phase)/2;
        //return 4*Phase01*(1-Phase01);
        return 2*(1+Phase)*(1-Phase)-1;
    }
};

#endif // PSEUDOSIN_H
