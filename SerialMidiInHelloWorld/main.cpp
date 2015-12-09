#include "mbed.h"
#include "SerialBuffer.h"
#include "RawMidiByteParser.h"

//Serial pc6(PA_11, PA_12);//serial 6

const int RecieveBufferSize = 128;

Serial pc6(D8, D2);//serial 1??

CSerialBuffer<int, RecieveBufferSize> g_SerialBuffer;

void OnRecieve()
{
    g_SerialBuffer.Write(pc6.getc());
}

class CMidiHandler
{
public:
    CMidiHandler()
    {}

    void OnNoteOff(int Note, int Velocity, int Channel)
    {
        printf("NoteOff %d (%d) - Ch %d \r\n", Note, Velocity, Channel);
    }

    void OnNoteOn(int Note, int Velocity, int Channel)
    {
        printf("NoteOn %d (%d) - Ch %d \r\n", Note, Velocity, Channel);
    }

    void OnControlChange(int Controller, int Value, int Channel)
    {
        printf("ControlChange %d (%d) - Ch %d \r\n", Controller, Value, Channel);
    }
};

int main()
{
    DigitalOut myled(LED1);
    Serial pc(USBTX, USBRX);

    pc.printf("Serial Midi In hello world...\r\n");

    CRawMidiParser Parser;
    CMidiHandler Handler;

    //int Baud = 9600;
    int Baud = 31250;//midi baudrate
    //int Baud = 115200;
    pc6.baud(Baud);
    printf("Baud rate = %d \r\n", Baud);

    pc6.attach(&OnRecieve);

    while(1)
    {
        wait(0.02f);

        int Available = g_SerialBuffer.Available();
        if(0<Available)
        {
            printf("OverWrites: %d \r\n", g_SerialBuffer.NumOverWrites());

            printf("Read: %d \r\n", Available);
            for(int idx = 0; idx<Available; ++idx)
            {
                int Byte = g_SerialBuffer.Read();
                printf("0x%x ", Byte);
                Parser.Parse(Byte, Handler);
            }
            printf("\r\n");
        }
    }
}
