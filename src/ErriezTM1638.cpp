/*
 * MIT License
 *
 * Copyright (c) 2018-2020 Erriez
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
 * \file ErriezTM1638.cpp
 * \brief TM1638 library for Arduino
 * \details
 *      Source:         https://github.com/Erriez/ErriezTM1638
 *      Documentation:  https://erriez.github.io/ErriezTM1638
 */

#include "ErriezTM1638.h"

/*!
 * \brief TM1638 constructor
 * \details
 *      Constructor with pin arguments: C-D-E (Clock, Data, Enable)
 * \param clkPin TM1638 CLK pin.
 * \param dioPin TM1638 DIO pin.
 * \param stbPin TM1638 STB pin.
 */
TM1638::TM1638(uint8_t clkPin, uint8_t dioPin, uint8_t stbPin, bool displayOn, uint8_t brightness) :
    _displayOn(displayOn), _brightness(brightness & 0x07)
{
#ifdef __AVR
    // Calculate bit and port register for fast pin read and writes (AVR targets only)
    _clkPort = digitalPinToPort(clkPin);
    _dioPort = digitalPinToPort(dioPin);
    _stbPort = digitalPinToPort(stbPin);

    _clkBit = digitalPinToBitMask(clkPin);
    _dioBit = digitalPinToBitMask(dioPin);
    _stbBit = digitalPinToBitMask(stbPin);
#else
    // Use the slow digitalRead() and digitalWrite() functions for non-AVR targets
    _dioPin = dioPin;
    _clkPin = clkPin;
    _stbPin = stbPin;
#endif
}

/*!
 * \brief Initialize TM1638 controller.
 */
void TM1638::begin()
{
    // Initialize pins
    TM1638_STB_HIGH();
    TM1638_DIO_LOW();
    TM1638_CLK_LOW();

    // Set pin mode
    TM1638_DIO_OUTPUT();
    TM1638_CLK_OUTPUT();
    TM1638_STB_OUTPUT();

    // Write _displayOn and _brightness to display control register
    writeDisplayControl();

    // Data write with auto address increment
    writeCommand(TM1638_CMD_DATA | TM1638_DATA_WRITE | TM1638_DATA_AUTO_INC_ADDR);
}

/*!
 * \brief Disable pins.
 */
void TM1638::end()
{
    TM1638_DIO_INPUT();
    TM1638_CLK_INPUT();
    TM1638_STB_INPUT();

    TM1638_DIO_LOW();
    TM1638_CLK_LOW();
    TM1638_STB_LOW();
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
 *    Display brightness value 0..7
 */
void TM1638::setBrightness(uint8_t brightness)
{
    if (brightness <= 7) {
        _brightness = brightness;

        writeDisplayControl();
    }
}

/*!
 * \brief Turn all LED's off.
 */
void TM1638::clear()
{
    TM1638_STB_LOW();
    writeByte((uint8_t)(TM1638_CMD_ADDR | 0x00));
    for (uint8_t i = 0; i < TM1638_NUM_GRIDS; i++) {
        writeByte(0x00);
    }
    TM1638_STB_HIGH();
}

/*!
 * \brief Write display register
 * \param address Display address 0x00..0x0F
 * \param data Value 0x00..0xFF
 */
void TM1638::writeData(uint8_t address, uint8_t data)
{
    if (address <= TM1638_NUM_GRIDS) {
        // Write byte to display register
        TM1638_STB_LOW();
        writeByte((uint8_t)(TM1638_CMD_ADDR | address));
        writeByte(data);
        TM1638_STB_HIGH();
    }
}

/*!
 * \brief Write buffer to multiple display registers
 * \details
 *    Write buffer to TM1638 with auto address increment
 * \param address
 *    Display address 0x00..0x0F
 * \param buf
 *    Buffer
 * \param len
 *    Buffer length
 */
void TM1638::writeData(uint8_t address, const uint8_t *buf, uint8_t len)
{
    if ((address + len) <= TM1638_NUM_GRIDS) {
        // Write buffer to display registers
        TM1638_STB_LOW();
        writeByte((uint8_t)(TM1638_CMD_ADDR | address));
        for (uint8_t i = 0; i < len; i++) {
            writeByte(buf[i]);
        }
        TM1638_STB_HIGH();
    }
}

/*!
 * \brief Get key states.
 * \return One or more buttons. One bit per button.
 */
uint32_t TM1638::getKeys()
{
    uint32_t keys = 0;

    // Read 4 Bytes key-scan registers
    TM1638_STB_LOW();
    writeByte(TM1638_CMD_DATA | TM1638_DATA_READ_KEYS);
    for (uint8_t i = 0; i < 4; i++) {
        keys |= ((uint32_t)readByte() << (i * 8));
    }
    TM1638_STB_HIGH();

    return keys;
}

// -------------------------------------------------------------------------------------------------
/*!
 * \brief Write display control register.
 * \details
 *      Set brightness and display on/off
 */
void TM1638::writeDisplayControl()
{
    writeCommand((uint8_t)(TM1638_CMD_CTRL |
                           (_displayOn ? TM1638_CTRL_DISPLAY_ON : TM1638_CTRL_DISPLAY_OFF) |
                           _brightness));
}

/*!
 * \brief Write command to TM1638.
 */
void TM1638::writeCommand(uint8_t cmd)
{
    TM1638_STB_LOW();
    writeByte(cmd);
    TM1638_STB_HIGH();
}

/*!
 * \brief Write byte to TM1638.
 * \param data 8-bit value.
 */
void TM1638::writeByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++) {
        TM1638_CLK_LOW();
        TM1638_PIN_DELAY();
        if (data & (1 << i)) {
            TM1638_DIO_HIGH();
        } else {
            TM1638_DIO_LOW();
        }
        TM1638_CLK_HIGH();
        TM1638_PIN_DELAY();
    }
}

/*!
 * \brief Read byte from TM1638.
 * \return 8-bit value.
 */
uint8_t TM1638::readByte()
{
    uint8_t data = 0;

    TM1638_DIO_INPUT();
    for (uint8_t i = 0; i < 8; i++) {
        TM1638_CLK_LOW();
        TM1638_PIN_DELAY();
        if (TM1638_DIO_READ()) {
            data |= (1 << i);
        }
        TM1638_CLK_HIGH();
        TM1638_PIN_DELAY();
    }
    TM1638_DIO_OUTPUT();

    return data;
}
