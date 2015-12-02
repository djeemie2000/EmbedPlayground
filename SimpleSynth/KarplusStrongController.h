#pragma once

#include "mbed.h"
#include "PolyKarplusStrong.h"
#include "MCP4822.h"
#include "IntBipolarLUTShaper.h"

class CKarplusStrongController
{
public:
    CKarplusStrongController(Serial& SerialComm, MCP4822& Mcp4822);

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
    static const int m_NumOperators = 8;

    Serial& m_SerialComm;
    MCP4822& m_Mcp4822;
    Ticker m_Ticker;

    float m_Damp;
    float m_Excitation;
    float m_AttackMilliSeconds;
    float m_FrequencyL;
    synthlib::CPolyKarplusStrong<float, m_SamplingFrequency/m_MinFrequency, m_NumOperators> m_KarplusStrong;
    synthlib::CSelector<m_NumOperators> m_OperatorSelector;
    isl::CBipolarLUTShaper<int> m_SaturationShaper;

};
