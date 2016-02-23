/*
 * This is the original mbed example
 */

#include "mbed.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);

AnalogOut myAnOut(A2);

void SawOut(float Frequency, float PulseWidth)
{
    pc.printf("SawOut\r\n");

    Timer t;
    t.start();

    float SamplingFrequency = 40000.0f;
    int PeriodMicroSeconds = 1000000/SamplingFrequency;

    float PhaseIncr = Frequency/SamplingFrequency;

    pc.printf("Fs=%f F=%f Incr=%f\r\n", SamplingFrequency, Frequency, PhaseIncr);

    float Phase = 0;
    for(int Repeat = 0; Repeat<SamplingFrequency; ++Repeat)
    {
        //float Out = Phase<PulseWidth?1:0;
        float Out = Phase;
        myAnOut.write(Out);

        Phase += PhaseIncr;
        if(1.0f<Phase)
        {
            Phase -= 1;
        }

        wait_us(PeriodMicroSeconds);
    }
    myAnOut.write(0.5f);

    t.stop();
    pc.printf("Elapsed %d uSec \r\n", t.read_us());
}

int main()
{
    pc.printf("AnalogOut HelloWorld!");

    float PulseWidth = 0.1f;
    float Frequency = 220.0f;
    while(true)
    {
        SawOut(Frequency, PulseWidth);
        PulseWidth+=0.1f;
        if(1<PulseWidth)
        {
            PulseWidth = 0.1f;
        }
        //wait(1.0f);
    }

  int i = 0;
  while(true)
  {
    myled = 1;
    myAnOut = 1;
    wait(0.5);

    myled = 0;
    myAnOut = 0;
    wait(0.5);

    pc.printf("i: %i\n\r", i);
    ++i;
  }

  return 0;
}
