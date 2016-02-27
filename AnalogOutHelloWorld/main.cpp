/*
 * This is the original mbed example
 */

#include "mbed.h"
#include "AnalogOutBuffer.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);

AnalogOut myAnOut(A2);// A2 or D13

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
    for(int Repeat = 0; Repeat<SamplingFrequency/20; ++Repeat)
    {
        float Out = Phase<PulseWidth?1:0;
        //float Out = Phase;
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


static const int g_SamplingFrequency = 96000;//44100;//?
static const int g_AnalogOutBufferSize = g_SamplingFrequency/10;
CAnalogOutBuffer<g_AnalogOutBufferSize> g_AnalogOutBuffer(D13);

void WriteToBuffer()
{
    static const int WriteBlockSize = 256;
    auto& Buffer = g_AnalogOutBuffer.GetBuffer();
    int NumAvailable = Buffer.AvailableForWrite();
    if(WriteBlockSize<=NumAvailable)
    {
        // write 512 samples
        for(int idx = 0; idx<WriteBlockSize; ++idx)
        {
            Buffer.Write(idx<<8);// scale 512 (9 bits) to 16 bits ?
        }
        //pc.printf("Write block, %d available \r\n", NumAvailable);
    }
    else
    {
        pc.printf("Cannot write block, %d available \r\n", NumAvailable);
        wait_ms(20);//too large??
    }
}

void TestBuffer()
{
    pc.printf("Testing buffer...\r\n");

    WriteToBuffer();
    g_AnalogOutBuffer.Start(g_SamplingFrequency);

    int Repeat = 0;
    while(true)
    {
        WriteToBuffer();
        ++Repeat;
        if(Repeat%24==0)
        {
            pc.printf("Write to buffer %d underruns %d \r\n", Repeat, g_AnalogOutBuffer.GetOverRuns());
        }
    }
}

void TestAnalogOut()
{
    pc.printf("Testing AnalogOut...\r\n");

    float PulseWidth = 0.1f;
    float Frequency = 220.0f;
    while(true)
    {
        SawOut(Frequency, PulseWidth);
        PulseWidth+=0.09f;
        if(1<PulseWidth)
        {
            PulseWidth = 0.1f;
        }

        Frequency = Frequency*1.325f + 11.34f;
        if(Frequency > 1520.0f)
        {
            Frequency /= 14.162f;
        }
        //wait(1.0f);
    }
}

int main()
{
    pc.printf("AnalogOut HelloWorld!");

    TestBuffer();
    //TestAnalogOut();

    //-----------------------------------

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
  //-----------------------------------

  return 0;
}
