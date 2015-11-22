#include "KarplusStrongController.h"
#include "MidiNoteFrequencies.h"

CKarplusStrongController::CKarplusStrongController(mbed::Serial &SerialComm, MCP4822 &Mcp4822)
 : m_SerialComm(SerialComm)
 , m_Mcp4822(Mcp4822)
 , m_Ticker()
 , m_Damp(0.9f)
 , m_Excitation(0.7f)
 , m_AttackMilliSeconds(1.0f)
 , m_FrequencyL(110.0f)
 , m_FrequencyR(110.0f)
 , m_KarplusStrong()
{
}

void CKarplusStrongController::Init()
{
    m_Damp = 0.9f;
    m_Excitation = 0.65f;
    m_AttackMilliSeconds = 1.0f;
    m_FrequencyL = GetMidiNoteFrequencyMilliHz(40)/1000.0f;
    m_FrequencyR = 1.01*m_FrequencyL;
}

void CKarplusStrongController::Test()
{
    m_SerialComm.printf("Testing KarplusStrong...\r\n");

    TestDacSpeed();

    TestTickSpeed();
}

void CKarplusStrongController::Start()
{
    m_SerialComm.printf("Starting KarplusStrong...\r\n");

    float PeriodMicroSeconds = 1000000.0f/m_SamplingFrequency;
    m_SerialComm.printf("Fs = %d Ps = %f\r\n", m_SamplingFrequency, PeriodMicroSeconds);

    m_Ticker.attach_us(this, &CKarplusStrongController::Tick, PeriodMicroSeconds);

    m_SerialComm.printf("Started!\r\n");
}

void CKarplusStrongController::Stop()
{
    m_Ticker.detach();
}

namespace
{

// convert and clamp to [0, 4096[
template<class T>
int ConvertandClamp(T In)
{
    int Value = In*2048;
    Value += 2048;
    if(Value<0)
    {
        Value = 0;
    }
    else if(4095<Value)
    {
        Value = 4095;
    }
    return Value;
}

}

void CKarplusStrongController::Tick()
{
    float Out = m_KarplusStrong();
    int OutValue = ConvertandClamp(Out);

    m_Mcp4822.writeAB(OutValue, OutValue);
}

void CKarplusStrongController::Process(int Value1, int Value2, int Value3)
{
    //TODO
    m_SerialComm.printf("Running... \r\n");

    m_Damp = (Value1+128)/256.0f;
    m_SerialComm.printf("Damp %f \r\n", m_Damp);

    int MidiNote = Value2+64;
    m_FrequencyL = GetMidiNoteFrequencyMilliHz(MidiNote)/1000.0f;
    m_FrequencyR = 1.01*m_FrequencyL;
    m_SerialComm.printf("MidiNote %d Freq %f %f \r\n", MidiNote, m_FrequencyL, m_FrequencyR);

    m_Excitation = (Value3+128)/256.0f;
    m_SerialComm.printf("Excitation %f \r\n", m_Excitation);

    //??
    m_KarplusStrong.Excite(m_Excitation, m_FrequencyL, m_Damp, m_AttackMilliSeconds);

    //wait(1.0f);
}

void CKarplusStrongController::TestDacSpeed()
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

void CKarplusStrongController::TestTickSpeed()
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
