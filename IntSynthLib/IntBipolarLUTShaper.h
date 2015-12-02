#pragma once

namespace isl
{

template<class T>
class CBipolarLUTShaper
{
public:
    CBipolarLUTShaper()
        : m_LUT(0)
        , m_LUTSize(0)
    {}

    /*!
     * \brief SetLUT set LUT , takes NO ownership
     * \param LUT
     * \param LUTSize
     */
    void SetLUT(const T* LUT, int LUTSize)
    {
        m_LUT = LUT;
        m_LUTSize = LUTSize;
    }

    T operator()(T In) const
    {
        return (0<=In) ? ( In<m_LUTSize ? m_LUT[In] : m_LUT[m_LUTSize-1] ) : (-In<m_LUTSize ? -m_LUT[-In] : -m_LUT[m_LUTSize-1] );
    }

private:
    const T* m_LUT;
    int m_LUTSize;
};

}

