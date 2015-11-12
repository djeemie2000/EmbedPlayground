#include "mbed.h"
#include "MCP4822.h"

MCP4822 MCP(SPI_MOSI, SPI_SCK, SPI_CS);    // MOSI, SCLK, nCS

Serial mySerial(USBTX, USBRX); // tx, rx
Timer myTimer;


void TestDacSpeed()
{
    myTimer.reset();
    myTimer.start();
    int NumRepeats = 40000;
    for(int Repeat = 0; Repeat<NumRepeats; ++Repeat)
    {
        MCP.writeAB((8*Repeat)%4096, (8*Repeat)%4096);
    }
    myTimer.stop();
    printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());
}

int main()
{
    while(1)
    {
        TestDacSpeed();
    }
}
