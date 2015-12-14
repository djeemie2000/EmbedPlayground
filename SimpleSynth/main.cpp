#include "mbed.h"
#include "MCP4822.h"
#include "SimpleOscillatorController.h"
#include "CombNoiseController.h"
#include "IntSimpleOscillatorController.h"
#include "KarplusStrongController.h"
#include "SerialBuffer.h"
#include "RawMidiByteParser.h"

// audio out using MCP4822 DAC (12bit)
MCP4822 g_MCP(SPI_MOSI, SPI_SCK, SPI_CS);    // MOSI, SCLK, nCS

// serial port for (debug) info
Serial g_Serial(USBTX, USBRX); // tx, rx

// Midi input : serial port + buffer
Serial g_MidiInSerial(D8, D2);//serial 1
static const int RecieveBufferSize = 128;
CSerialBuffer<int, RecieveBufferSize> g_SerialBuffer;

void OnRecieve()
{
    g_SerialBuffer.Write(g_MidiInSerial.getc());
}

// interrupts
static const int InterruptBufferSize = 16;
CSerialBuffer<int, InterruptBufferSize> g_InterruptBuffer;

template<int N>
void OnInterrupt()
{
    g_InterruptBuffer.Write(N);
}

int main()
{
    g_Serial.printf("SimpleSynth... \r\n \r\n");

    InterruptIn Trigger1(D4);
    Trigger1.mode(PullUp);
    InterruptIn Trigger2(D5);
    Trigger2.mode(PullUp);
    //InterruptIn Trigger3(D6);
    //InterruptIn Trigger4(D7);


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

    Trigger1.rise(&OnInterrupt<1>);
    Trigger2.rise(&OnInterrupt<2>);
    //Trigger3.rise(&OnInterrupt<3>);
    //Trigger4.rise(&OnInterrupt<4>);


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

            //printf("Serial Midi in %d \r\n", Available);
        }

        int AvailableInterrupts = g_InterruptBuffer.Available();
        if(0<AvailableInterrupts)
        {
            for(int idx = 0; idx<AvailableInterrupts; ++idx)
            {
                int Interrupt = g_InterruptBuffer.Read();
                if(0==g_InterruptBuffer.NumOverWrites())
                {
                    Controller.OnInterrupt(Interrupt);
                    //std::printf("Interrupt %d \r\n", Interrupt);
                }
            }
        }

        wait_ms(1);//necessary???
    }

    Controller.Stop();
}
