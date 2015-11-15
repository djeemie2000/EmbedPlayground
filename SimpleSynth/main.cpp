#include "mbed.h"
#include "MCP4822.h"
#include "SimpleOscillatorController.h"
#include "CombNoiseController.h"

MCP4822 g_MCP(SPI_MOSI, SPI_SCK, SPI_CS);    // MOSI, SCLK, nCS

Serial g_Serial(USBTX, USBRX); // tx, rx

void TestDacSpeed()
{
    Timer g_Timer;
    g_Timer.reset();
    g_Timer.start();
    int NumRepeats = 40000;
    for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
    {
        g_MCP.writeAB((8*Repeat)%4096, (8*Repeat)%4096);
    }
    g_Timer.stop();
    g_Serial.printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, g_Timer.read_us());

    g_Serial.printf("---------------\r\n");
}

int main()
{
    g_Serial.printf("SimpleSynth... \r\n \r\n");

    AnalogIn In0(A0);
    AnalogIn In1(A1);
    AnalogIn In2(A2);

    // self test
    CCombNoiseController Controller(g_Serial, g_MCP);
    //CSimpleOscillatorController Controller(g_Serial, g_MCP);

    Controller.Init();
    Controller.Test();

    wait(2.0f);
    Controller.Start();

    while(1)
    {
        int Val0 = In0.read_u16();
        int Val1 = In1.read_u16();
        int Val2 = In2.read_u16();

        Controller.Process(Val0, Val1, Val2);
    }

    Controller.Stop();
}
