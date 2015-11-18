#pragma once

#include "mbed.h"
#include "IntSimpleOscillator.h"
#include "IntOnePoleFilter.h"
#include "MCP4822.h"

class CIntSimpleOscillatorController
{
public:
    CIntSimpleOscillatorController(Serial& SerialComm, MCP4822& Mcp4822);

    void Init();
    void Test();
    void Start();
    void Stop();

    void Tick(); // to be called by ticker
    void Process(int Value0, int Value1, int Value2); // to be called in loop

private:
    void TestDacSpeed();
    void TestOscillatorSpeed();
    void TestTickSpeed(int Operator);


    const int m_SamplingFrequency;

    Serial& m_SerialComm;
    MCP4822& m_Mcp4822;
    Ticker m_Ticker;

    static const int McpScale = 12;
    static const int OscillatorScale = 16;
    isl::CSimpleOscillator<OscillatorScale> m_Oscillator;
    static const int LPFScale = 12;
    isl::COnePoleLowPassFilter<int, LPFScale> m_LPF;
};
