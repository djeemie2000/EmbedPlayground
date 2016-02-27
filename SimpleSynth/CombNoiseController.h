#pragma once

#include "mbed.h"
#include "Noise.h"
#include "FourPoleFilter.h"
#include "CombFilter.h"
#include "MCP4822.h"

class CCombNoiseController
{
public:
    CCombNoiseController(Serial& SerialComm, MCP4822& Mcp4822);

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

    static const int m_SamplingFrequency = 50000;
    static const int m_MinFrequency = 40;

    Serial& m_SerialComm;
    MCP4822& m_Mcp4822;
    Ticker m_Ticker;

    CNoise<float> m_Exciter;
    CFourPoleLowPassFilter<float> m_LPF;
    float m_Feedback;
    float m_FrequencyL;
    float m_FrequencyR;
    CCombFilter<float, m_SamplingFrequency, m_MinFrequency> m_CombFilterL;
    CCombFilter<float, m_SamplingFrequency, m_MinFrequency> m_CombFilterR;
};
