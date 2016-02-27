#include "KarplusStrongController.h"
#include "MidiNoteFrequencies.h"
#include "LUTSaturationTanH.h"

CKarplusStrongController::CKarplusStrongController(mbed::Serial &SerialComm, MCP4822 &Mcp4822)
 : m_SerialComm(SerialComm)
 , m_Mcp4822(Mcp4822)
 , m_Ticker()
 , m_Damp(0.9f)
 , m_Excitation(0.7f)
 , m_AttackMilliSeconds(1.0f)
 , m_Frequency(110.0f)
 , m_Pan(0.0f)
 , m_Gate(false)
 , m_KarplusStrong()
 , m_OperatorSelector()
 , m_SaturationShaper()
{
}

void CKarplusStrongController::Init()
{
    m_Damp = 0.9f;
    m_Excitation = 0.65f;
    m_AttackMilliSeconds = 1.0f;
    m_Frequency = GetMidiNoteFrequencyMilliHz(40)/1000.0f;
    m_Pan = 0.0f;
    m_Gate = false;
    m_SaturationShaper.SetLUT(isl::LUT_SaturationTanH_4096, 4096);
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

void CKarplusStrongController::Tick()
{
    float OutL = 0.0f;
    float OutR = 0.0f;
    m_KarplusStrong(OutL, OutR);

    int OutLValue = 2048 + m_SaturationShaper(2048*OutL);
    int OutRValue = 2048 + m_SaturationShaper(2048*OutR);

    m_Mcp4822.writeAB(OutLValue, OutRValue);
}

void CKarplusStrongController::Process(int Value1, int Value2, int Value3, int Value4)
{
    m_Damp = (Value1+128)/256.0f;

    int MidiNote = Value2+64;
    float Freq = GetMidiNoteFrequencyMilliHz(MidiNote)/1000.0f;
    m_Frequency = Freq;

    m_Excitation = (Value3+128)/256.0f;

    bool Gate = 0<Value4;
    if(!m_Gate && Gate)
    {
        int SelectedOperator = m_OperatorSelector.Select();
        m_KarplusStrong.Excite(SelectedOperator, m_Excitation, m_Frequency, m_Damp, m_AttackMilliSeconds, m_Pan);

        m_SerialComm.printf("Operator %d : MidiNote %d\r\n", 1+SelectedOperator, MidiNote);
    }
    m_Gate = Gate;
    m_SerialComm.printf("Freq %f Damp %f Exci %f \r\n", m_Frequency, m_Damp, m_Excitation);
}

void CKarplusStrongController::OnNoteOff(int /*Note*/, int /*Velocity*/, int /*Channel*/)
{
    // ignored
}

void CKarplusStrongController::OnNoteOn(int Note, int /*Velocity*/, int /*Channel*/)
{
    int MidiNote = Note;
    m_Frequency = GetMidiNoteFrequencyMilliHz(MidiNote)/1000.0f;

    // ??
    // use velocity for excitation?
    // use average of param + velocity?

    int SelectedOperator = m_OperatorSelector.Select();
    m_KarplusStrong.Excite(SelectedOperator, m_Excitation, m_Frequency, m_Damp, m_AttackMilliSeconds, m_Pan);

    m_SerialComm.printf("Operator %d : MidiNote %d Freq %f \r\n", 1+SelectedOperator, MidiNote, m_Frequency);
}

void CKarplusStrongController::OnControlChange(int Controller, int Value, int Channel)
{
    bool Handled = true;
    if(Controller==17)
    {
        m_Damp = (1+Value)/128.0f;
    }
    else if(Controller==18)
    {
        m_Excitation = (1+Value)/128.0f;
    }
    else if(Controller==19)
    {
        // 0 to ~1600
        m_AttackMilliSeconds = Value*Value/10.0f;
    }
    else if(Controller==20)
    {
        m_Pan = (Value-64)/128.0f;
    }
    else
    {
        Handled = false;
    }
    if(Handled)
    {
        m_SerialComm.printf("Damp %f Exci %f Atck %f \r\n", m_Damp, m_Excitation, m_AttackMilliSeconds);
    }
}

void CKarplusStrongController::OnInterrupt(int Interrupt)
{
    if(Interrupt==1)
    {
        int SelectedOperator = m_OperatorSelector.Select();
        m_KarplusStrong.Excite(SelectedOperator, m_Excitation, m_Frequency, m_Damp, m_AttackMilliSeconds, m_Pan);

        m_SerialComm.printf("Operator %d : MidiNote ? Freq %f \r\n", 1+SelectedOperator, m_Frequency);
    }
    else if(Interrupt==2)
    {
        int SelectedOperator = m_OperatorSelector.Select();
        m_KarplusStrong.Excite(SelectedOperator, m_Excitation, m_Frequency*2, m_Damp, m_AttackMilliSeconds, m_Pan);

        m_SerialComm.printf("Operator %d : MidiNote ? Freq %f \r\n", 1+SelectedOperator, m_Frequency*2);
    }
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
