/* MCP4822 library 
 * Copyright (c) 2008-2010, Lerche
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mbed.h"
#include "MCP4822.h"

using namespace mbed;

MCP4822::MCP4822(PinName mosi, PinName sclk, PinName ncs) : _spi(mosi, NC, sclk, NC), _ncs(ncs) {
    _init();
}

void MCP4822::_init() {
    _spi.format(16, 0);  // Setup the SPI
    _spi.frequency(32000000);//32MHz (default 1MHz)
    return;
}

void MCP4822::writeA(int ValueA)
{
    _ncs = 0;       // Chipselect the device.
    _spi.write((ValueA & 0x0FFF) | 0x3000);
    _ncs = 1;
    return;
}

void MCP4822::writeB(int ValueB)
{
    _ncs = 0;       // Chipselect the device.
    _spi.write((ValueB & 0x0FFF) | 0xB000);
    _ncs = 1;
    return;
}

void MCP4822::writeAB(int ValueA, int ValueB)
{
    _ncs = 0;       // Chipselect the device.
    _spi.write((ValueA & 0x0FFF) | 0x3000);
    _ncs = 1;

    _ncs = 0;
    _spi.write((ValueB & 0x0FFF) | 0xB000);
    _ncs = 1;
}
        

void MCP4822::write(char chan, int value) {
    value = value & 0x0FFF;
    switch(chan){
        case('A'):
            value = value | 0x1000;
            break; 
        case('B'):
            value = value | 0x9000;
            break;
        case('a'):
            value = value | 0x1000;
            break; 
        case('b'):
            value = value | 0x9000;
            break;
        case(0x01):
            value = value | 0x1000;
            break; 
        case(0x02):
            value = value | 0x9000;
            break;
        default: 
            break;        
    }
    _ncs = 0;
    _spi.write(value);
    _ncs = 1;
    return;
}

void MCP4822::shdn() {
    _ncs = 0;
    _spi.write(0x0000);
    _ncs = 1;
    return;
}

