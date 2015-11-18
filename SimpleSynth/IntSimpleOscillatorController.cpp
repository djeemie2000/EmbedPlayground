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
    int Value = (Out>>(OscillatorScale-McpScale)) + 2048;
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

void CIntSimpleOscillatorController::Process(int Value1, int Value2, int Value3)
{
    //TODO
    m_SerialComm.printf("Running... \r\n");

    int Operator = Value1*(isl::COperatorFactory<OscillatorScale>::Size()-1)>>16;
    m_Oscillator.SelectOperator(Operator);
    m_SerialComm.printf("Operator %d %s \r\n", Operator, isl::COperatorFactory<OscillatorScale>::AvailableOperatorNames()[Operator].c_str());

    int MidiNote = Value2>>9;//16 to 7 bits
    int Freq = GetMidiNoteFrequencyMilliHz(MidiNote);
    m_Oscillator.SetFrequency(Freq);
    m_SerialComm.printf("MidiNote %d Freq %f \r\n", MidiNote, Freq*0.001f);

    int CutOff = Value3>>4;//16bits to 12bits
    m_LPF.SetParameter(CutOff);
    m_SerialComm.printf("CutOff %d \r\n", CutOff);

    wait(1.0f);
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
