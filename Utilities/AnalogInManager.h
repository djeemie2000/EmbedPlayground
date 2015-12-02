#pragma once

#include "mbed.h"
#include "StablePot.h"

template<int Resolution>
class CAnalogInManager
{
public:
    static const int Size = 6;//TODO more flexible?

    CAnalogInManager()
     : m_PinName()
    {
        //TODO flexible way to use other pins
        m_PinName[0] = A0;
        m_PinName[1] = A1;
        m_PinName[2] = A2;
        m_PinName[3] = A3;
        m_PinName[4] = A4;
        m_PinName[5] = A5;

        for(int Pin = 0; Pin<Size; ++Pin)
        {
            m_PinAnalogInEnabled[Pin] = false;
            m_StableValue[Pin] = -1;//unused
            m_Value[Pin] = 0;
        }
    }

    void Begin(bool Enabled[Size], int AverageSize = 4)
    {
        m_AverageSize = AverageSize;
        m_Counter = 0;

        for(int Pin = 0; Pin<Size; ++Pin)
        {
            m_PinAnalogInEnabled[Pin] = Enabled[Pin];

            if(m_PinAnalogInEnabled[Pin])
            {
                m_StableValue[Pin] = 0;
                analogin_init(&(m_PinAnalogIn[Pin]), m_PinName[Pin]);
            }
            else
            {
                // unused pins are used as digital out and set to zero
                m_StableValue[Pin] = -1;//unused
                gpio_init_out(&(m_PinDigitalOut[Pin]), m_PinName[Pin]);
                gpio_write(&(m_PinDigitalOut[Pin]), 0);
            }
            m_Value[Pin] = 0;
        }
    }

    /*
     * Update should be called regulary,
     * preferably with some time inbetween,
     * e.g. once per millisecond
     */
    void Update()
    {
        ++m_Counter;
        for(int Pin = 0; Pin<Size; ++Pin)
        {
            if(m_PinAnalogInEnabled[Pin])
            {
                int Immediate = analogin_read_u16(&(m_PinAnalogIn[Pin]));
                m_Value[Pin] += Immediate;

                if(m_AverageSize<=m_Counter)
                {
                    int Average = m_Value[Pin]/m_AverageSize;
                    m_Value[Pin] = 0;

                    m_StableValue[Pin] = m_Stable[Pin].Update(Average);
                }
            }
        }
        if(m_AverageSize<=m_Counter)
        {
            m_Counter = 0;
        }

    }

    int Get(int Pin)
    {
        return m_StableValue[Pin];
    }

private:
    static const int AdcResolution = 16;

    PinName m_PinName[Size];
    bool    m_PinAnalogInEnabled[Size];
    int     m_StableValue[Size];

    int     m_Value[Size];
    CStablePots<AdcResolution, Resolution> m_Stable[Size];
    gpio_t  m_PinDigitalOut[Size];
    analogin_t m_PinAnalogIn[Size];

    int m_AverageSize;
    int m_Counter;

};
