#include "SimpleOscillatorController.h"
#include "MidiNoteFrequencies.h"

CSimpleOscillatorController::CSimpleOscillatorController(mbed::Serial &SerialComm, MCP4822 &Mcp4822)
 : m_SamplingFrequency(50000)
 , m_SerialComm(SerialComm)
 , m_Mcp4822(Mcp4822)
 , m_Ticker()
 , m_Oscillator(m_SamplingFrequency)
 , m_LPF()
{

}

void CSimpleOscillatorController::Init()
{
    m_Oscillator.SelectWaveform(4);
    m_Oscillator.SetFrequency(110.0f);
    m_LPF.SetParameter(1.0f);
}

void CSimpleOscillatorController::Test()
{
    m_SerialComm.printf("Testing SimpleOscillator...\r\n");

    TestTickSpeed();
}

void CSimpleOscillatorController::Start()
{
    m_SerialComm.printf("Starting SimpleOscillator...\r\n");

    float PeriodMicroSeconds = 1000000.0f/m_SamplingFrequency;
    m_SerialComm.printf("Fs = %d Ps = %f\r\n", m_SamplingFrequency, PeriodMicroSeconds);

    m_Ticker.attach_us(this, &CSimpleOscillatorController::Tick, PeriodMicroSeconds);

    m_SerialComm.printf("Started!\r\n");
}

void CSimpleOscillatorController::Stop()
{
    m_Ticker.detach();
}

void CSimpleOscillatorController::Tick()
{
    float Out = m_LPF(m_Oscillator());

    // convert and clamp to [0, 4096[
    int Value = Out*2048;
    Value += 2048;
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

void CSimpleOscillatorController::Process(int Value1, int Value2, int Value3)
{
    //TODO
    m_SerialComm.printf("Running... \r\n");

    int Operator = Value1*(CSelectableOperatorFactory::Size()-1)>>16;
    m_Oscillator.SelectWaveform(Operator);
    m_SerialComm.printf("Operator %d %s \r\n", Operator, CSelectableOperatorFactory::SelectionList()[Operator].c_str());

    int MidiNote = Value2>>9;//16 to 7 bits
    float Freq = GetMidiNoteFrequencyMilliHz(MidiNote)/1000.0f;
    m_Oscillator.SetFrequency(Freq);
    m_SerialComm.printf("MidiNote %d Freq %f \r\n", MidiNote, Freq);

    float CutOff = Value3/65536.0f;
    m_LPF.SetParameter(CutOff);
    m_SerialComm.printf("CutOff %f \r\n", CutOff);

    wait(2.0f);
}

void CSimpleOscillatorController::TestTickSpeed()
{
    m_SerialComm.printf("Test Tick speed\r\n");
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
