/*
 * This is the original mbed example
 */

#include "mbed.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);

AnalogOut myAnOut(A2);

int main()
{
  int i = 0;
  while(true)
  {
    myled = 1;
    myAnOut = 1;
    wait(0.5);

    myled = 0;
    myAnOut = 0;
    wait(0.5);

    pc.printf("i: %i\n\r", i);
    ++i;
  }

  return 0;
}
