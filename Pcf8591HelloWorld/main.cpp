/*
 * This is the original mbed example
 */

#include "mbed.h"
#include "PCF8591.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);

void I2cDiscovery(I2C& i2c_bus)
{
    pc.printf("Scanning I2C addresses... \r\n");
    for(int Address = 2; Address<256; Address+=1)
    {
        char Data[2];
        Data[0] = 0x00;
        Data[1] = 0x00;
        int Error = i2c_bus.write(Address, Data, 1, false);
        if(Error==0)
        {
            pc.printf("Found I2C device at address %d \r\n", Address);
        }
        else
        {
            //pc.printf("No I2C device at address 0x%h \r\n", Address);
        }
    }
    pc.printf("Scanning done\r\n");
}

int main()
{
    pc.printf("Pcf8591 HelloWorld\r\n");

    // I2C Communication
    I2C i2c_bus(I2C_SDA, I2C_SCL);

    I2cDiscovery(i2c_bus);

    wait(2.0f);

    PCF8591_AnalogIn AdcIn0(&i2c_bus, PCF8591_ADC0, PCF8591_SA0);
    //PCF8591_AnalogOut AdcOut(&i2c_bus, PCF8591_SA0);
    //PCF8591 AdcDac(&i2c_bus, 144);

    int i = 0;
    while(1)
    {
        // int OutVal = 63+(i%4)*64;
        // AdcDac.write(OutVal);
        //float OutVal = (i%4)*0.25f;

        // int Value = AdcDac.read(PCF8591_ADC0);
        //pc.printf("Read %d \r\n", Value);

        //AdcOut.write(OutVal);
        // pc.printf("Write %d \r\n", OutVal);

        int IntValue = AdcIn0.read_int();
        pc.printf("Read %d \r\n", IntValue);

        float Value = AdcIn0;
        pc.printf("Read %f \r\n", Value);

        myled = 1;
        wait(0.5);

        myled = 0;
        wait(0.5);

        pc.printf("i: %i\n\r", i);
        ++i;
    }
}
