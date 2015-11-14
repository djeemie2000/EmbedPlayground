#include "mbed.h"

int main()
{
    DigitalOut myled(LED1);
    Serial pc(USBTX, USBRX);
    Serial pc2(SERIAL_TX, SERIAL_RX);//same as USBTX,USBRX
    Serial pc6(PA_11, PA_12);//serial 6
    Serial pc1(PA_15, PB_7);//serial 1 -> does not work?

    pc1.baud(9600);
    pc6.baud(9600);

    int Counter = 0;
    while(1)
    {
        myled = 1;
        wait(0.3);
        myled = 0;
        wait(0.3);
        pc.printf("Serial USB : i: %i\n\r", Counter);
        pc1.printf("Serial 1   : i: %i\n\r", Counter);
        pc2.printf("Serial 2   : i: %i\n\r", Counter);
        pc6.printf("Serial 6   : i: %i\n\r", Counter);
        ++Counter;
    }
}
