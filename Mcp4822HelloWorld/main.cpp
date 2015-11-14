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
        int ValueA = (8*Repeat)%4096;
        int ValueB = (4*Repeat)%4096;

        MCP.writeAB(ValueA, ValueB);
    }
    myTimer.stop();
    printf("The time taken for %d repeats was %d microseconds \r\n", NumRepeats, myTimer.read_us());
}

int main()
{
    printf("MCP4822 HelloWorld \r\n \r\n");
    while(1)
    {
        TestDacSpeed();
    }
}
