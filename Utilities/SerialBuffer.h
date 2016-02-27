#pragma once

template<class T, int BufferSize>
class CSerialBuffer
{
public:
    CSerialBuffer()
        : m_Buffer()
        , m_WriteCounter(0)
        , m_ReadCounter(0)
    {}

    // number available for read
    int Available() const
    {
        return m_WriteCounter - m_ReadCounter;
    }

    int AvailableForWrite()
    {
        return BufferSize - (m_WriteCounter-m_ReadCounter);
    }

    T Read()
    {
        if(m_ReadCounter<m_WriteCounter)
        {
            int Index = m_ReadCounter%BufferSize;
            ++m_ReadCounter;
            return m_Buffer[Index];
        }
        return -1;//??
    }

    void Write(T Value)
    {
        int Index = m_WriteCounter%BufferSize;
        ++m_WriteCounter;
        m_Buffer[Index] = Value;
    }

    int NumOverWrites()
    {
        int Num = m_WriteCounter-m_ReadCounter-BufferSize;
        return 0<Num ? Num : 0;
    }

    // number available for write
    int NumUnderWrites()
    {
        int Num = m_WriteCounter-m_ReadCounter-BufferSize;
        return Num<0 ? -Num : 0;
    }

private:
    T m_Buffer[BufferSize];
    int m_WriteCounter;
    int m_ReadCounter;
};
