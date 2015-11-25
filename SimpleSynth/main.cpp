#include "mbed.h"
#include "MCP4822.h"
#include "SimpleOscillatorController.h"
#include "CombNoiseController.h"
#include "IntSimpleOscillatorController.h"
#include "KarplusStrongController.h"
#include "RotaryEncoder.h"

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

    CRotaryEncoder In0;
    In0.Begin(D2, D3);
    CRotaryEncoder In1;
    In1.Begin(D4, D5);
    CRotaryEncoder In2;
    In2.Begin(D6, D7);

    // self test
    //CCombNoiseController Controller(g_Serial, g_MCP);
    //CSimpleOscillatorController Controller(g_Serial, g_MCP);
    //CIntSimpleOscillatorController Controller(g_Serial, g_MCP);
    CKarplusStrongController Controller(g_Serial, g_MCP);

    Controller.Init();
    Controller.Test();

    wait(2.0f);

//    while(true)
//    {
//        g_Serial.printf("... \r\n");
//        wait(5.0f);
//    }

    Controller.Start();

    int PrevVal0 = 0;
    int PrevVal1 = 0;
    int PrevVal2 = 0;

    while(1)
    {
        In0.Read();
        In1.Read();
        In2.Read();

        int Val0 = In0.GetPosition();//16 bit
        int Val1 = In1.GetPosition();
        int Val2 = In2.GetPosition();

        if(Val0!=PrevVal0 || Val1!=PrevVal1 || Val2!=PrevVal2)
        {
            PrevVal0 = Val0;
            PrevVal1 = Val1;
            PrevVal2 = Val2;
            //g_Serial.printf("%d %d %d \r\n", Val0, Val1, Val2);

            Controller.Process(Val0, Val1, Val2);
        }
        wait_ms(1);//necessary?
    }

    Controller.Stop();
}
