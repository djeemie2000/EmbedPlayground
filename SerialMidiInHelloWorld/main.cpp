#include "mbed.h"

//Serial pc6(PA_11, PA_12);//serial 6

const int RecieveBufferSize = 32;
char g_RecieveBuffer[RecieveBufferSize];
int g_RecieveCounterBuffer[RecieveBufferSize];
int g_RecieveCounter;

Serial pc6(D8, D2);//serial 1??

void OnRecieve()
{
    int Index = g_RecieveCounter%RecieveBufferSize;
    g_RecieveBuffer[Index] = pc6.getc();
    g_RecieveCounterBuffer[Index] = g_RecieveCounter;
    ++g_RecieveCounter;
}

void PrintRecieveBuffer()
{
    printf("Buffer: \r\n");
    for(int idx = 0; idx<RecieveBufferSize; ++idx)
    {
        printf("%d : 0x%x \r\n", g_RecieveCounterBuffer[idx], g_RecieveBuffer[idx]);
    }
    printf("\r\n");
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

    g_RecieveCounter = 0;
    pc6.attach(&OnRecieve);

    while(1)
    {
        int Cntr = g_RecieveCounter;
        wait(0.2f);
        if(Cntr!=g_RecieveCounter)
        {
            PrintRecieveBuffer();
        }
    }
}
