#include "CombNoiseController.h"
#include "MidiNoteFrequencies.h"

CCombNoiseController::CCombNoiseController(mbed::Serial &SerialComm, MCP4822 &Mcp4822)
 : m_SamplingFrequency(50000)
 , m_SerialComm(SerialComm)
 , m_Mcp4822(Mcp4822)
 , m_Ticker()
 , m_Exciter()
 , m_LPF()
 , m_Feedback(0.0f)
 , m_Frequency(110.0f)
 , m_CombFilter(m_SamplingFrequency, 100.0f)
{
}

void CCombNoiseController::Init()
{
    m_LPF.SetParameter(0.5f);
    m_Feedback = 0.5f;
    m_Frequency = GetMidiNoteFrequencyMilliHz(40)/1000.0f;
}

void CCombNoiseController::Test()
{
    m_SerialComm.printf("Testing CombNoise...\r\n");

    TestTickSpeed();
}

void CCombNoiseController::Start()
{
    m_SerialComm.printf("Starting CombNoise...\r\n");

    float PeriodMicroSeconds = 1000000.0f/m_SamplingFrequency;
    m_SerialComm.printf("Fs = %d Ps = %f\r\n", m_SamplingFrequency, PeriodMicroSeconds);

    m_Ticker.attach_us(this, &CCombNoiseController::Tick, PeriodMicroSeconds);

    m_SerialComm.printf("Started!\r\n");
}

void CCombNoiseController::Stop()
{
    m_Ticker.detach();
}

void CCombNoiseController::Tick()
{
    float Out = m_CombFilter(m_LPF(m_Exciter()), m_Feedback, m_Frequency);

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

void CCombNoiseController::Process(int Value1, int Value2, int Value3)
{
    //TODO
    m_SerialComm.printf("Running... \r\n");

    m_Feedback = Value1/65536.0f;
    m_SerialComm.printf("Feedback %f \r\n", m_Feedback);

    int MidiNote = Value2>>9;//16 to 7 bits
    m_Frequency = GetMidiNoteFrequencyMilliHz(MidiNote)/1000.0f;
    m_SerialComm.printf("MidiNote %d Freq %f \r\n", MidiNote, m_Frequency);

    float CutOff = Value3/65536.0f;
    m_LPF.SetParameter(CutOff);
    m_SerialComm.printf("CutOff %f \r\n", CutOff);

    wait(1.0f);
}

void CCombNoiseController::TestTickSpeed()
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
