#include "mbed.h"
#include "MCP4822.h"
#include "SimpleOscillatorController.h"
#include "CombNoiseController.h"
#include "IntSimpleOscillatorController.h"
#include "KarplusStrongController.h"
#include "SerialBuffer.h"
#include "RawMidiByteParser.h"

MCP4822 g_MCP(SPI_MOSI, SPI_SCK, SPI_CS);    // MOSI, SCLK, nCS

Serial g_Serial(USBTX, USBRX); // tx, rx

Serial g_MidiInSerial(D8, D2);//serial 1

static const int RecieveBufferSize = 128;
CSerialBuffer<int, RecieveBufferSize> g_SerialBuffer;

void OnRecieve()
{
    g_SerialBuffer.Write(g_MidiInSerial.getc());
}

int main()
{
    g_Serial.printf("SimpleSynth... \r\n \r\n");

//    DigitalIn Button1(D8);

    CRawMidiParser MidiParser;

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

    const int MidiBaudRate = 31250;
    g_MidiInSerial.baud(MidiBaudRate);
    Controller.Start();
    g_MidiInSerial.attach(&OnRecieve);

    while(1)
    {
        int Available = g_SerialBuffer.Available();
        if(0<Available)
        {
            for(int idx = 0; idx<Available; ++idx)
            {
                int RawMidiByte = g_SerialBuffer.Read();
                MidiParser.Parse(RawMidiByte, Controller);
            }

            //printf("Serial %d \r\n", Available);
        }

        wait_ms(1);//necessary???
    }

    Controller.Stop();
}
