#pragma once

#include "mbed.h"
#include "SimpleOscillator.h"
#include "OnePoleFilter.h"
#include "MCP4822.h"

class CSimpleOscillatorController
{
public:
    CSimpleOscillatorController(Serial& SerialComm, MCP4822& Mcp4822);

    void Init();
    void Test();
    void Start();
    void Stop();

    void Tick(); // to be called by ticker
    void Process(int Value1, int Value2, int Value3); // to be called in loop

private:
    void TestDacSpeed();
    void TestOscillatorSpeed();
    void TestTickSpeed();


    const int m_SamplingFrequency;

    Serial& m_SerialComm;
    MCP4822& m_Mcp4822;
    Ticker m_Ticker;

    CSimpleOscillator<float> m_Oscillator;
    COnePoleLowPassFilter<float> m_LPF;
};
