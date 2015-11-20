#include "IntSimpleOscillatorController.h"
#include "MidiNoteFrequencies.h"

CIntSimpleOscillatorController::CIntSimpleOscillatorController(mbed::Serial &SerialComm, MCP4822 &Mcp4822)
 : m_SamplingFrequency(50000)
 , m_SerialComm(SerialComm)
 , m_Mcp4822(Mcp4822)
 , m_Ticker()
 , m_Oscillator(m_SamplingFrequency)
 , m_LPF()
{

}

void CIntSimpleOscillatorController::Init()
{
    m_Oscillator.SelectOperator(4);
    m_Oscillator.SetFrequency(110*1000);
    m_LPF.SetParameter(1<<LPFScale);
}

void CIntSimpleOscillatorController::Test()
{
    m_SerialComm.printf("Testing SimpleOscillator...\r\n");

    TestDacSpeed();

    for(int Operator = 0; Operator<isl::COperatorFactory<OscillatorScale>::Size(); ++Operator)
    {
        TestTickSpeed(Operator);
    }
}

void CIntSimpleOscillatorController::Start()
{
    m_SerialComm.printf("Starting SimpleOscillator...\r\n");

    float PeriodMicroSeconds = 1000000.0f/m_SamplingFrequency;
    m_SerialComm.printf("Fs = %d Ps = %f\r\n", m_SamplingFrequency, PeriodMicroSeconds);

    m_Ticker.attach_us(this, &CIntSimpleOscillatorController::Tick, PeriodMicroSeconds);

    m_SerialComm.printf("Started!\r\n");
}

void CIntSimpleOscillatorController::Stop()
{
    m_Ticker.detach();
}

void CIntSimpleOscillatorController::Tick()
{
    int Out = m_LPF(m_Oscillator());//16 bits

    // convert and clamp to [0, 4096[
    int Value = (Out>>(OscillatorScale-McpScale-1)) + 2048;
    if(Value<0)
    {
        Value = 0;
    }
    else if(4095<Value)
    {
        Value = 4095;
    }

    m_Mcp4822.writeAB(Value, Value);
}

namespace
{

int Clamp(int Value, int Min, int Max)
{
    if(Value<Min)
        return Min;

    if(Max<Value)
        return Max;

    return Value;
}

}

void CIntSimpleOscillatorController::Process(int Value0, int Value1, int Value2)
{
    //TODO
    m_SerialComm.printf("Running... \r\n");

    //int Operator = Value1*(isl::COperatorFactory<OscillatorScale>::Size()-1)>>16;
    int Operator = Clamp(Value0, 0, isl::COperatorFactory<OscillatorScale>::Size());
    m_Oscillator.SelectOperator(Operator);
    m_SerialComm.printf("Operator %d %s \r\n", Operator, isl::COperatorFactory<OscillatorScale>::AvailableOperatorNames()[Operator].c_str());

    //int MidiNote = Value2>>9;//16 to 7 bits
    int MidiNote = Value1+64;
    int Freq = GetMidiNoteFrequencyMilliHz(MidiNote);
    m_Oscillator.SetFrequency(Freq);
    m_SerialComm.printf("MidiNote %d Freq %f \r\n", MidiNote, Freq*0.001f);

    //int CutOff = Value3>>4;//16bits to 12bits
    int CutOff = Clamp(Value2*16, -2048, 2048) + 2048;//12bits
    m_LPF.SetParameter(CutOff);
    m_SerialComm.printf("CutOff %d \r\n", CutOff);

    //wait(1.0f);
}

void CIntSimpleOscillatorController::TestDacSpeed()
{
    m_SerialComm.printf("Test Dac speed\r\n");

    Timer l_Timer;
    l_Timer.reset();
    l_Timer.start();
    int NumRepeats = m_SamplingFrequency;
    int Value = 2048;
    for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
    {
        m_Mcp4822.writeAB(Value, Value);
    }
    l_Timer.stop();

    m_SerialComm.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, l_Timer.read_us());
    m_SerialComm.printf("---------------\r\n");
}

void CIntSimpleOscillatorController::TestTickSpeed(int Operator)
{
    m_SerialComm.printf("Test Tick speed\r\n");
    m_SerialComm.printf("Operator = %s \r\n", isl::COperatorFactory<16>::AvailableOperatorNames()[Operator].c_str());
    m_Oscillator.SelectOperator(Operator);
    Timer l_Timer;
    l_Timer.reset();
    l_Timer.start();
    int NumRepeats = m_SamplingFrequency;
    for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
    {
        Tick();
    }
    l_Timer.stop();

    m_SerialComm.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, l_Timer.read_us());
    m_SerialComm.printf("---------------\r\n");
}
