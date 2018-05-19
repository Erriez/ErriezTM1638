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

/*!
 * \brief TM1638 library for Arduino
 * \details Source: https://github.com/Erriez/ErriezTM1638
 * \file TM1638.cpp
 */

#include "TM1638.h"

/*!
 * \brief TM1638 constructor
 * \param dioPin TM1638 DIO pin.
 * \param sclPin TM1638 SCL pin.
 * \param stbPin TM1638 STB pin.
 */
TM1638::TM1638(uint8_t dioPin, uint8_t sclPin, uint8_t stbPin) :
    _dioPin(dioPin), _clkPin(sclPin), _stbPin(stbPin), _displayOn(true), _brightness(5)
{
    // Initialize pins
    digitalWrite(_stbPin, HIGH);
    digitalWrite(_dioPin, LOW);
    digitalWrite(_clkPin, LOW);

    // Set pin mode
    pinMode(_dioPin, OUTPUT);
    pinMode(_clkPin, OUTPUT);
    pinMode(_stbPin, OUTPUT);

    // Clear display
    clear();

    // Write _displayOn and _brightness to display control register
    writeDisplayControl();
}

/*!
 * \brief Turn Display on.
 */
void TM1638::displayOn()
{
    _displayOn = true;

    writeDisplayControl();
}

/*!
 * \brief Turn display off.
 */
void TM1638::displayOff()
{
    _displayOn = false;

    writeDisplayControl();
}

/*!
 * \brief Set brightness LED's.
 * \param brightness
 */
void TM1638::setBrightness(uint8_t brightness)
{
    if (brightness <= 7) {
        _brightness = brightness;

        writeDisplayControl();
    }
}

/*!
 * Turn all LED's off.
 */
void TM1638::clear()
{
    writeCommand(TM1638_WRITE_DATA);

    digitalWrite(_stbPin, LOW);

    // Clear all display registers to turn all LED's off
    writeByte(TM1638_DISPLAY_ADDR);
    for (uint8_t i = 0; i < 16; i++) {
        writeByte(0);
    }

    digitalWrite(_stbPin, HIGH);
}

/*!
 * \brief Get key states.
 * \return One or more buttons. One bit per button.
 */
uint32_t TM1638::getKeyScan()
{
    uint32_t keys = 0;

    digitalWrite(_stbPin, LOW);

    // Read 4 key-scan registers
    writeByte(TM1638_READ_KEYS);
    for (uint8_t i = 0; i < 4; i++) {
        keys |= ((uint32_t)readByte() << (i * 8));
    }

    digitalWrite(_stbPin, HIGH);

    return keys;
}

/*!
 * \brief Write display control.
 */
void TM1638::writeDisplayControl()
{
    // Write to display control register
    writeCommand(TM1638_WRITE_DISPLAY_CTRL |
                 (_displayOn ? 0x08 : 0x00) |
                 _brightness);
}

/*!
 * \brief Write display register
 * \param address Display address 0x00..0x0F
 * \param data Value 0x00..0xFF
 */
void TM1638::writeDisplayRegister(uint8_t address, uint8_t data)
{
    if (address <= 0x0F) {
        // Disable auto address increment
        writeCommand(TM1638_ADDRESS_FIXED);

        // Write to display register
        digitalWrite(_stbPin, LOW);
        writeByte(TM1638_DISPLAY_ADDR | address);
        writeByte(data);
        digitalWrite(_stbPin, HIGH);
    }
}

/*!
 * \brief Write command.
 * \param cmd Please refer to the datasheet for a list with supported commands.
 */
void TM1638::writeCommand(uint8_t cmd)
{
    // Write command
    digitalWrite(_stbPin, LOW);
    writeByte(cmd);
    digitalWrite(_stbPin, HIGH);
}

/*!
 * \brief Read byte from TM1638.
 * \return 8-bit value.
 */
uint8_t TM1638::readByte()
{
    uint8_t data = 0;

    // Set DIO pin to input
    digitalWrite(_dioPin, LOW);
    pinMode(_dioPin, INPUT);

    // Read 8-bit
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(_clkPin, LOW);
        if (digitalRead(_dioPin)) {
            data |= (1 << i);
        }
        digitalWrite(_clkPin, HIGH);
    }

    // Restore DIO pin to output
    pinMode(_dioPin, OUTPUT);

    return data;
}

/*!
 * \brief Write byte to TM1638.
 * \param data 8-bit value.
 */
void TM1638::writeByte(uint8_t data)
{
    // Write 8-bit
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(_clkPin, LOW);

        if (data & (1 << i)) {
            digitalWrite(_dioPin, HIGH);
        } else {
            digitalWrite(_dioPin, LOW);
        }

        digitalWrite(_clkPin, HIGH);
    }
}
