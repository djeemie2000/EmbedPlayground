#pragma once

template<int InScale, int OutScale>
class CStablePots
{
public:
    CStablePots()
        : m_Out(0)
    {}

    int Update(int In)
    {
        int Immediate = In>>InToOutScale;
        if( Immediate<m_Out )
        {
            m_Out = (In+Threshold)>>InToOutScale;
        }
        else if(m_Out<Immediate)
        {
            m_Out = (In-Threshold)>>InToOutScale;
        }
        return m_Out;
    }

private:
    static const int InToOutScale = InScale-OutScale;
    static const int ThresholdScale = InToOutScale-2;// 1/4
    static const int Threshold = 1<<ThresholdScale;

    int m_Out;
};

