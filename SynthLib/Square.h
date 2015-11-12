#ifndef SQUARE_H
#define SQUARE_H

template<class T>
class CQuadratic
{
public:
    T operator()(const T& Phase) const
    {
        // (1+Phase)*(1+Phase)/4
        return Phase<0 ? -Phase*Phase : Phase*Phase;
    }
};

#endif // SQUARE_H
