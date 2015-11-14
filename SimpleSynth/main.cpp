#include "mbed.h"
#include "MCP4822.h"
#include "SimpleOscillator.h"
#include "OnePoleFilter.h"

MCP4822 MCP(SPI_MOSI, SPI_SCK, SPI_CS);    // MOSI, SCLK, nCS

Serial mySerial(USBTX, USBRX); // tx, rx
Timer myTimer;


void TestDacSpeed()
{
    myTimer.reset();
    myTimer.start();
    int NumRepeats = 40000;
    for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
    {
        MCP.writeAB((8*Repeat)%4096, (8*Repeat)%4096);
    }
    myTimer.stop();
    printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());

    printf("---------------\r\n");
}

void TestOscillatorSpeed()
{
    const int SamplingFrequency = 40000;
    CSimpleOscillator<float> Oscillator(SamplingFrequency);

    for(std::size_t Operator = 0; Operator<CSelectableOperatorFactory::SelectionList().size(); ++Operator)
    {
        Oscillator.SelectWaveform(Operator);
        Oscillator.SetFrequency(110.0f);//faster if not always recalculating phase step from frequency
        myTimer.reset();
        myTimer.start();
        float Sum = 0;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Sum += Oscillator();
        }
        myTimer.stop();
        printf("Operator %s -> %f \r\n", CSelectableOperatorFactory::SelectionList()[Operator].c_str(), Sum);
        printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());
    }

    printf("---------------\r\n");
}

void TestLowPassFilterSpeed()
{
    const int SamplingFrequency = 40000;
    CSimpleOscillator<float> Oscillator(SamplingFrequency);
    Oscillator.SelectWaveform(0);
    Oscillator.SetFrequency(110.0f);
    COnePoleLowPassFilter<float> LPF;
    COnePoleHighPassFilter<float> HPF;
    CMultiStageFilter<float, COnePoleLowPassFilter<float>, 4> MLPF;

    {
        LPF.SetParameter(0.5);
        myTimer.reset();
        myTimer.start();
        float Sum = 0;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Sum += LPF(Oscillator());
        }
        myTimer.stop();
        printf("Operator %s -> LPF -> %f \r\n", CSelectableOperatorFactory::SelectionList()[0].c_str(), Sum);
        printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());
    }

    {
        myTimer.reset();
        myTimer.start();
        float Sum = 0;
        float Parameter = 0.5f;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Parameter += 0.0001f;
            if(1.0f<=Parameter)
            {
                Parameter = 0.5f;
            }
            Sum += LPF(Oscillator(), Parameter);
        }
        myTimer.stop();
        printf("Operator %s -> LPF(Parameter) -> %f \r\n", CSelectableOperatorFactory::SelectionList()[0].c_str(), Sum);
        printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());
    }

    {
        MLPF.SetStages(4);
        myTimer.reset();
        myTimer.start();
        float Sum = 0;
        float Parameter = 0.5f;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Parameter += 0.0001f;
            if(1.0f<=Parameter)
            {
                Parameter = 0.5f;
            }
            Sum += MLPF(Oscillator(), Parameter);
        }
        myTimer.stop();
        printf("Operator %s -> MLPF(Parameter) -> %f \r\n", CSelectableOperatorFactory::SelectionList()[0].c_str(), Sum);
        printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());
    }

    {
        LPF.SetParameter(0.5);
        myTimer.reset();
        myTimer.start();
        float Sum = 0;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Sum += LPF(LPF(LPF(LPF(Oscillator()))));
        }
        myTimer.stop();
        printf("Operator %s -> LPF x4 -> %f \r\n", CSelectableOperatorFactory::SelectionList()[0].c_str(), Sum);
        printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());
    }

    printf("---------------\r\n");
}

int main()
{
    printf("SimpleSynth... \r\n \r\n");

    while(1)
    {
        TestDacSpeed();

        TestOscillatorSpeed();

        TestLowPassFilterSpeed();

        wait(4.0f);
    }
}
