#ifndef TRIANGLE_H
#define TRIANGLE_H

template<class T>
class CTriangle
{
public:
    CTriangle(){}
    T operator()(const T& Phase) const
    {
        // Phase < 0 : -1 -> -1 ; -0.5 -> 0 ; 0 -> 1
        // 0 < Phase : 0 -> 1 ; 0.5 -> 0 ; 1 -> -1
        // => 1 + 2*Phase / 1 - 2*Phase
        return Phase<0 ? 1+2*Phase : 1-2*Phase;
    }
};

#endif // TRIANGLE_H
