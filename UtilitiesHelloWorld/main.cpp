#include "mbed.h"
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
    float NormalizedDeviation = Deviation/float(NumSamples);
    mySerial.printf("Adc deviation = %f \r\n", NormalizedDeviation);

    mySerial.printf("Done \r\n");
}

int main()
{
    mySerial.printf("Utilities HelloWorld \r\n \r\n");

    const int Resolution = 8;
    CAnalogInManager<Resolution> Manager;
    bool Enabled[CAnalogInManager<Resolution>::Size] = {true, false, false, false, false, false};
    int AverageSize = 4;
    Manager.Begin(Enabled, AverageSize);

    int PrevValue = 0;
    while(true)
    {
        Manager.Update();

        int Value = Manager.Get(0);
        int Changed = PrevValue==Value ? 0 : 1;
        PrevValue = Value;

        if(Changed)
        {
            mySerial.printf("-> %d \r\n", Value);
        }

        wait_us(100);
    }

}
