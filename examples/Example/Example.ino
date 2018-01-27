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

#include <Arduino.h>
#include <TM1638.h>

// Connect display pins to the Arduino DIGITAL pins
#define DIO_PIN   2
#define SCL_PIN   3
#define STB_PIN   4

TM1638 tm1638(DIO_PIN, SCL_PIN, STB_PIN);

uint32_t keysLast = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("TM1638 example"));

  // Turn display off (All LED's off)
  tm1638.displayOff();

  // Set brightness 0..7
  tm1638.setBrightness(3);

  // Turn display on
  tm1638.displayOn();

  // Clear display
  tm1638.clear();

  // Write segment LED's to the first display register
  // The LED's turned on depends on the connection of your board
  // Experiment with the registers 0x00..0x0F value 0x00..0xff to display
  // numbers and characters
  tm1638.writeDisplayRegister(0x01, 0x01);
}

void loop()
{
  uint32_t keys;

  // Read 32-bit keys
  keys = tm1638.getKeyScan();

  // Check key down
  if (keysLast != keys) {
    keysLast = keys;

    if (keys) {
      Serial.println(F("Key down"));

      // Write segment LED's to first display register
      tm1638.writeDisplayRegister(0, 0b00111111);
    } else {
      Serial.println(F("Key up"));

      // Write segment LED's to first display register
      tm1638.writeDisplayRegister(0, 0b00000110);
    }
  }
}
