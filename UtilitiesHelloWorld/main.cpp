#include "mbed.h"
#include "StablePot.h"

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

    //    while(1)
    //    {
    //        {
    //            mySerial.printf("Setting outputs as digital out \r\n");

    //            DigitalOut Out1(A1);
    //            Out1.write(0);
    //            DigitalOut Out2(A2);
    //            Out2.write(0);
    //            DigitalOut Out3(A3);
    //            Out3.write(0);
    //            DigitalOut Out4(A4);
    //            Out4.write(0);
    //            DigitalOut Out5(A5);
    //            Out5.write(0);

    //            wait(1.0f);

    //            TestAdc(10);
    //            wait(5.0f);
    //        }

    //        {
    //            mySerial.printf("Setting outputs as analog in \r\n");

    //            AnalogIn Out1(A1);
    //            AnalogIn Out2(A2);
    //            AnalogIn Out3(A3);
    //            AnalogIn Out4(A4);
    //            AnalogIn Out5(A5);

    //            wait(1.0f);

    //            TestAdc(10);
    //            wait(5.0f);
    //        }
    //    }

    DigitalOut Out1(A1);
    Out1.write(0);
    DigitalOut Out2(A2);
    Out2.write(0);
    DigitalOut Out3(A3);
    Out3.write(0);
    DigitalOut Out4(A4);
    Out4.write(0);
    DigitalOut Out5(A5);
    Out5.write(0);

    AnalogIn AIn1(A0);
    CStablePots<16, 8> StablePot1;

    int PrevStable = 0;
    //int Immediate = 0;
    while(1)
    {
        const int NumMeasurements = 4;
        int Immediate =0;
        for(int Repeat = 0; Repeat<NumMeasurements; ++Repeat)
        {
            Immediate += AIn1.read_u16();
            wait_ms(1);
        }
        Immediate /= NumMeasurements;

        //Immediate = (Immediate*15 + AIn1.read_u16())/16;

        int Stable = StablePot1.Update(Immediate);

        int Changed = PrevStable==Stable ? 0 : 1;
        PrevStable = Stable;

        if(Changed)
        {
            mySerial.printf("%d : %d -> %d \r\n", Changed, Immediate, Stable);
        }
        //wait(0.010f);
    }
}
