#include "mbed.h"

//Serial pc6(PA_11, PA_12);//serial 6
Serial pc6(D8, D2);//serial 1??

int main()
{
    DigitalOut myled(LED1);
    Serial pc(USBTX, USBRX);

    pc.printf("Serial Midi In hello world...\r\n");


    int Baud = 9600;
    //int Baud = 31250;/midi baudrate
    pc6.baud(Baud);

    printf("Baud rate = %d \r\n", Baud);

    int Counter = 0;
    while(1)
    {
        const int ReadBufferSize = 3;
        char ReadBuffer[ReadBufferSize];

        uint32_t idx = 0;
        while(idx < ReadBufferSize)
        {
            while(!pc6.readable()); // busy waiting for data available
            ReadBuffer[idx++] = pc6.getc();
        }

        for(int idx = 0; idx<ReadBufferSize; ++idx)
        {
            printf("0x%x ", ReadBuffer[idx]);
        }
        printf("\r\n");

        ++Counter;
    }
}
