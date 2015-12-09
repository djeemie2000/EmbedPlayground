#include "mbed.h"
#include "SerialBuffer.h"
#include "RawMidiByteParser.h"
#include "LogMidiHandler.h"

//Serial pc6(PA_11, PA_12);//serial 6

const int RecieveBufferSize = 128;

Serial pc6(D8, D2);//serial 1??

CSerialBuffer<int, RecieveBufferSize> g_SerialBuffer;

void OnRecieve()
{
    g_SerialBuffer.Write(pc6.getc());
}

int main()
{
    DigitalOut myled(LED1);
    Serial pc(USBTX, USBRX);

    pc.printf("Serial Midi In hello world...\r\n");

    CRawMidiParser Parser;
    CLogMidiHandler Handler;

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
