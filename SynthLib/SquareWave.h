#ifndef SQUAREWAVE_H
#define SQUAREWAVE_H

template<class T>
class CSquareWave
{
public:
    CSquareWave()
     : m_Slope(0.5)
    {}

    void SetSlope(const T& Slope)
    {
        m_Slope = Slope;
    }

    T operator()(const T& Phase) const
    {
        return Phase<0 ? 1 - m_Slope*(1+Phase) : -1+m_Slope*Phase;
    }

private:
    T m_Slope;
};

#endif // SQUAREWAVE_H
