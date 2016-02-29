#pragma once

#include "mbed.h"

class CInterruptInManager
{
public:
    CInterruptInManager(PinName Pin)
        : m_InterruptIn(Pin)
        , m_RiseCount(0)
        , m_FallCount(0)
    {}

    void Start()
    {
        m_InterruptIn.rise(this, &CInterruptInManager::OnRise);
        m_InterruptIn.fall(this, &CInterruptInManager::OnFall);
    }

//    void Stop()
//    {
//        m_InterruptIn.
//    }

    void OnRise()
    {
        ++m_RiseCount;
    }

    void OnFall()
    {
        ++m_FallCount;
    }

    //!< reads current state (0 or 1)
    int Read()
    {
        return m_InterruptIn.read();
    }

    int RiseCount() const
    {
        return m_RiseCount;
    }

    int FallCount() const
    {
        return m_FallCount;
    }

    void Clear()
    {
        m_RiseCount = 0;
        m_FallCount = 0;//???
    }

private:
    InterruptIn m_InterruptIn;
    int m_RiseCount;
    int m_FallCount;
};

