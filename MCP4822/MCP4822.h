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

#ifndef MCP4822_H
#define MCP4822_H

/** Interface to a 12-bit SPI dual-output DAC */
class MCP4822 {
public:
     /** Constructor: MCP4822
      * 
      * @param MOSI - The SPI Data out pin  
      * @param SCLK - The SPI clock pin
      * @param /CS - The ChipSelect pin 
      */ 
    MCP4822 (PinName mosi, PinName sclk, PinName ncs);
     /** Write to the A-output, max 4095 (0xFFF) with 1 mV steps
      *  
      * @param ValueA The value to be written
      */ 
    void writeA (int ValueA);
     /** Write to the B-output, max 4095 (0xFFF) with 1 mV steps
      *
      * @param ValueB The valkue to be written
      */ 
    void writeB (int ValueB);
    
    void writeAB(int ValueA, int ValueB);
    
     /** Write to any output, accoding to letter.
      *
      *
      */
    void write(char chan, int value);
     /** Shutdown the output on specific channel
      *
      *
      */
    void shdn();
    
    private :    
        SPI _spi;
        DigitalOut _ncs;
        void _init();
  };

#endif
