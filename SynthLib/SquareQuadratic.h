#ifndef SQUAREQUADRATIC_H
#define SQUAREQUADRATIC_H

template<class T>
class CSquareQuadratic
{
public:
    CSquareQuadratic(){}

    T operator()(T Phase) const
    {
        // square wave with nonlinear decrease
        return Phase<0 ? 1 : 2*(1-Phase)*(1-Phase)-1;
    }
};

#endif // SQUAREQUADRATIC_H
