#include "mbed.h"
#include "MCP4822.h"
#include "SimpleOscillator.h"

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
}

void TestOscillatorSpeed()
{
    const int SamplingFrequency = 40000;
    CSimpleOscillator<float> Oscillator(SamplingFrequency);
    for(std::size_t Operator = 0; Operator<CSelectableOperatorFactory::SelectionList().size(); ++Operator)
    {
        Oscillator.SelectWaveform(Operator);
        myTimer.reset();
        myTimer.start();
        float Sum = 0;
        int NumRepeats = SamplingFrequency;
        for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
        {
            Sum += Oscillator(110.0f);
        }
        myTimer.stop();
        printf("Operator %s -> %f \r\n", CSelectableOperatorFactory::SelectionList()[Operator].c_str(), Sum);
        printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());
    }
}

int main()
{
    while(1)
    {
        TestDacSpeed();

        TestOscillatorSpeed();
    }
}
