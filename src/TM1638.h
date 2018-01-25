/*
 * MIT License
 *
 * Copyright (c) 2018 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* TM1638 library for Arduino
 * https://github.com/Erriez/ArduinoLibraryTM1638
 */

#ifndef TM1638_H__
#define TM1638_H__

#include <Arduino.h>

/* Command / register definitions

  MSB           LSB
   7 6 5 4 3 2 1 0
  -----------------
   0 1 - - - - - -	Data command
   1 0 - - - - - -	Display control command
   1 1 - - - - - -	Address command


  7.1 Data Command Set

  MSB           LSB
   7 6 5 4 3 2 1 0
  -----------------
   0 1 0 0 0 - 0 0  Write display data
   0 1 0 0 0 - 1 0	Read key scan data
   0 1 0 0 0 0 - -	Auto address increment
   0 1 0 0 0 1 - -	Fixed address


  7.2 Address command set

  MSB           LSB
   7 6 5 4 3 2 1 0
  -----------------
   1 1 0 - A A A A	Address 0x00..0x0F


  7.3 Display Control

  MSB           LSB
   7 6 5 4 3 2 1 0
  -----------------
   1 0 0 0 - 0 0 0	Set the pulse width of 1 / 16
   1 0 0 0 - 0 0 1	Set the pulse width of 2 / 16
   1 0 0 0 - 0 1 0	Set the pulse width of 4 / 16
   1 0 0 0 - 0 1 1	Set the pulse width of 10 / 16
   1 0 0 0 - 1 0 0	Set the pulse width of 11 / 16
   1 0 0 0 - 1 0 1	Set the pulse width of 12 / 16
   1 0 0 0 - 1 1 0	Set the pulse width of 13 / 16
   1 0 0 0 - 1 1 1	Set the pulse width of 14 / 16
   1 0 0 0 0 - - -	Display off
   1 0 0 0 1 - - -	Display on
*/

#define TM1638_WRITE_DISPLAY_ADDR_INC	  0x40
#define TM1638_WRITE_DISPLAY_ADDR_FIX	  0x44
#define TM1638_READ_KEYS				        0x42 /* Address increment */

#define TM1638_WRITE_DISPLAY_CTRL		    0x80

#define TM1638_ADDR						          0xc0

class TM1638
{
public:
    TM1638(uint8_t dioPin, uint8_t sclPin, uint8_t stbPin);

    virtual void displayOn();
    virtual void displayOff();
    virtual void setBrightness(uint8_t brightness);
    virtual void clear();
    virtual uint32_t getKeyScan();
    virtual void writeDisplayRegister(uint8_t address, uint8_t data);

protected:
    virtual void writeCommand(uint8_t cmd);
    virtual void writeDisplayControl();
    virtual uint8_t readByte();
    virtual void writeByte(uint8_t data);

private:
    uint8_t _dioPin;
    uint8_t _clkPin;
    uint8_t _stbPin;

    bool    _displayOn;
    uint8_t _brightness;
};

#endif // TM1638_H__
