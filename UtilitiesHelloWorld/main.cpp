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

int main()
{
    mySerial.printf("Utilities HelloWorld \r\n \r\n");

    const int Resolution = 8;
    CAnalogInManager<Resolution> Manager;
    bool Enabled[CAnalogInManager<Resolution>::Size] = {false, true, true, false, false, false};
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
