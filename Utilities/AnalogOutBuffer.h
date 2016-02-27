#pragma once

#include "mbed.h"
#include "SerialBuffer.h"

template<int BufferSize, class T = int>
class CAnalogOutBuffer
{
public:
    CAnalogOutBuffer(PinName AnalogOutPin)
     : m_Buffer()
     , m_Ticker()
     , m_AnalogOut(AnalogOutPin)
     , m_OverRuns(0)
    {}

    void Start(int SamplingFrequency)
    {
        timestamp_t PeriodMicroSeconds = 1000000.0f/SamplingFrequency;
        m_Ticker.attach_us(this, &CAnalogOutBuffer<BufferSize>::OnTick, PeriodMicroSeconds);
    }

    void Stop()
    {
        m_Ticker.detach();
    }

    void OnTick()
    {
        if(0<m_Buffer.Available())
        {
            T Value = m_Buffer.Read();
            // no cropping here?
            m_AnalogOut.write_u16(Value);
        }
        else
        {
            ++m_OverRuns;
        }
    }

    CSerialBuffer<T, BufferSize>& GetBuffer()
    {
        return m_Buffer;
    }

    int GetOverRuns() const
    {
        return m_OverRuns;
    }

private:
    CSerialBuffer<T, BufferSize> m_Buffer;
    Ticker m_Ticker;
    AnalogOut m_AnalogOut;
    int m_OverRuns;
};

