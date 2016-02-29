#include "mbed.h"
#include "dsp.h"
#include "StablePot.h"
#include "AnalogInManager.h"

Serial mySerial(USBTX, USBRX); // tx, rx

void TestAdc(int /*Resolution*/)
{
    mySerial.printf("Adc test pin %d \r\n", A0);

    //const int AdcResolution = 16;
    AnalogIn AIn1(A0);

    // calc average
    mySerial.printf("Measure average... \r\n");
    int Average = 0;
    for(int Repeat = 0; Repeat<2048; ++Repeat)
    {
        Average += AIn1.read_u16();
    }
    Average /= 2048;
    mySerial.printf("Adc average = %d \r\n", Average);

    const int NumSamples = 20000;
    mySerial.printf("Measure deviation over %d samples... \r\n", NumSamples);
    int Deviation = 0;
    for(int Repeat = 0; Repeat<NumSamples; ++Repeat)
    {
        int Value = AIn1.read_u16();
        //Deviation += std::abs(Value-Average);
        Deviation += (Value-Average)*(Value-Average);
    }
    float Variation = Deviation/float(NumSamples);
    mySerial.printf("Adc variation = %f \r\n", Variation);

    float StandardDeviation = 0;
    arm_sqrt_f32(Variation, &StandardDeviation);
    mySerial.printf("Adc std dev = %f \r\n", StandardDeviation);


    mySerial.printf("Done \r\n");
}

void TestAnalogInManager()
{
    const int Resolution = 8;
    CAnalogInManager<Resolution> Manager;
    bool Enabled[CAnalogInManager<Resolution>::Size] = {true, true, true, true, false, false};
    int AverageSize = 8;
    Manager.Begin(Enabled, AverageSize);

    int PrevValue[CAnalogInManager<Resolution>::Size] = {-2, -2, -2, -2, -2, -2};
    while(true)
    {
        Manager.Update();

        for(int Pin = 0; Pin<CAnalogInManager<Resolution>::Size; ++Pin)
        {
            int Value = Manager.Get(Pin);
            int Changed = PrevValue[Pin]==Value ? 0 : 1;
            PrevValue[Pin] = Value;

            if(Changed)
            {
                mySerial.printf("Pin %d -> %d \r\n", Pin, Value);
            }
        }

        wait_us(100);
    }
}

void TestDigitalIn()
{
    mySerial.printf("Test gate in...\r\n");

    DigitalIn GateIn(D7, PullUp);
    int PrevGate = 0;
    while(true)
    {
        int Gate = GateIn;
        if(Gate!=PrevGate)
        {
            mySerial.printf("Gate changed to %d \r\n", Gate);
            PrevGate  = Gate;
        }
//        wait_ms(1);
    }
}

class CInterruptHandler
{
public:
    CInterruptHandler()
        : m_Counter(0)
        , m_Interrupt(false)
    {}

    void OnInterrupt()
    {
        ++m_Counter;
        m_Interrupt = true;
    }

    bool Get() const
    {
        return m_Interrupt;
    }

    int Count() const
    {
        return m_Counter;
    }

    void Clear()
    {
        m_Interrupt = false;
        m_Counter = 0;
    }

private:
    int m_Counter;
    bool m_Interrupt;
};

void TestTriggerIn()
{
    mySerial.printf("Test trigger in...\r\n");

    CInterruptHandler Handler;
    InterruptIn TriggerIn(D7);
    TriggerIn.rise(&Handler, &CInterruptHandler::OnInterrupt);

    while(true)
    {
        if(Handler.Get())
        {
            // this is where we should sample the CV pitch?
            int Count = Handler.Count();
            mySerial.printf("Triggered %d \r\n", Count);
            Handler.Clear();
        }
        wait_ms(10);//medium wait
        //wait_ms(100);//long wait!!
    }
}

void TestAnalogIn()
{
    mySerial.printf("CV pitch (1V/oct) input test using pin %d \r\n", A5);

    //const int AdcResolution = 16;
    AnalogIn AIn1(A5);

    while(true)
    {
        int Value = AIn1.read_u16();
        const float ReferenceVoltage = 5.0f;//3.3f;
        const int Resolution = 1<<16;
        float Voltage = Value*ReferenceVoltage/Resolution;
        int MidiNote = 12*Voltage + 0.5f;
        mySerial.printf("%d = %f V wrt %f -> Note %d \r\n", Value, Voltage, ReferenceVoltage, MidiNote);
        wait_ms(500);
    }
}

int main()
{
    mySerial.printf("Utilities HelloWorld \r\n \r\n");

    TestAdc(12345);
    wait(2.0f);

    //TestAnalogIn();

    //TestDigitalIn();

    TestTriggerIn();

    TestAnalogInManager();
}
