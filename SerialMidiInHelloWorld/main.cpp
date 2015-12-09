#include "mbed.h"
#include "SerialBuffer.h"

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


    int Baud = 9600;
    //int Baud = 31250;/midi baudrate
    pc6.baud(Baud);
    printf("Baud rate = %d \r\n", Baud);

    pc6.attach(&OnRecieve);

    while(1)
    {
        wait(0.2f);

        int Available = g_SerialBuffer.Available();
        if(0<Available)
        {
            printf("OverWrites: %d \r\n", g_SerialBuffer.NumOverWrites());

            printf("Read: %d \r\n", Available);
            for(int idx = 0; idx<Available; ++idx)
            {
                printf("0x%x ", g_SerialBuffer.Read());
            }
            printf("\r\n");
        }
    }
}
