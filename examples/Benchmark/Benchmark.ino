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
 * \file Benchmark.ino
 * \brief TM1638 benchmark example
 * \details
 *    Required libraries:
 *       https://github.com/Erriez/ErriezTM1638
 *       https://github.com/Erriez/ErriezTimestamp
 */

#include <Arduino.h>

#include <ErriezTM1638.h>
#include <ErriezTimestamp.h>

// Connect display pins to the Arduino DIGITAL pins
#if defined(ARDUINO_ARCH_AVR)
#define TM1638_CLK_PIN      2
#define TM1638_DIO_PIN      3
#define TM1638_STB0_PIN     4
#define TEST_PIN            5
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI) || defined(ESP8266_WEMOS_D1MINI) || defined(ARDUINO_ESP8266_NODEMCU)
#define TM1638_CLK_PIN      D2
#define TM1638_DIO_PIN      D3
#define TM1638_STB0_PIN     D4
#define TEST_PIN            D5
#elif defined(ARDUINO_LOLIN32)
#define TM1638_CLK_PIN      0
#define TM1638_DIO_PIN      4
#define TM1638_STB0_PIN     5
#define TEST_PIN            16
#else
#error "May work, but not tested on this target"
#endif

// Create objects
TM1638 tm1638(TM1638_CLK_PIN, TM1638_DIO_PIN, TM1638_STB0_PIN);
TimestampMicros timestamp;


void setup()
{
    pinMode(TEST_PIN, OUTPUT);

    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    Serial.println(F("TM1638 benchmark example"));

    // Initialize TM1638
    tm1638.begin();
    tm1638.clear();

    uint8_t buf[16];
    for (uint8_t i = 0; i < 16; i++) {
        buf[i] = 0x3f;
    }

    delay(2000);

    digitalWrite(TEST_PIN, HIGH);

    Serial.print(F("getKeys(): "));
    timestamp.start();
    uint32_t keys = tm1638.getKeys();
    timestamp.print();

    delay(1);

    Serial.print(F("writeData(0, 0x7f): "));
    timestamp.start();
    tm1638.writeData(0x00, 0x7f);
    timestamp.print();

    delay(1);

    Serial.print(F("writeData(buf, 16): "));
    timestamp.start();
    tm1638.writeData(0x00, buf, sizeof(buf));
    timestamp.print();

    delay(1);

    Serial.print(F("Clear(): "));
    timestamp.start();
    tm1638.clear();
    timestamp.print();

    digitalWrite(TEST_PIN, LOW);
}

void loop()
{
}
