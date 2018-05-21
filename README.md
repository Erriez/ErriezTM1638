# Optimized TM1638 library for Arduino

This is a 3-pin serial TM1638 chip library for Arduino, optimized for size and speed. It supports a combined LED driver controller and key-scan interface.

![TM1638 chip](https://raw.githubusercontent.com/Erriez/ErriezTM1638/master/extras/TM1638_pins.jpg)

Displaying numbers, characters and reading keys depends on the hardware wiring and is not part of this library. A fully operational example for a board with 8 7-segment displays, 8 dual color LED's and 8 buttons which uses this library is available here: [JY-LKM1638](https://github.com/Erriez/ErriezLKM1638).

## Hardware

Connect power and 3 data pins to an Arduino board DIGITAL pins:
* VDD (Power 3.3V - 5V)
* GND (Ground)
* DIO (Bi-directional data input/output)
* STB (Chip select)
* CLK (Clock)

The following TM1638 pins should be connected to LED's and buttons in a matrix:  
* K1~K3 (Key-scan data input to read multiple key presses at the same time)
* SEG/GRID (Output for LED matrix)


### Pins

| Pin  | TM1638 | Arduino UNO / Nano / Pro Micro / Mega2560 / Leonardo | Node MCU | LOLIN32 |
| :--: | :----: | :--------------------------------------------------: | :------: | :-----: |
|  1   |  VCC   |                     5V (or 3.3V)                     |   GND    |   GND   |
|  2   |  GND   |                         GND                          |   3V3    |   3V3   |
|  3   |  CLK   |                   2 (Digital pin)                    |    D2    |    0    |
|  4   |  DIO   |                   3 (Digital pin)                    |    D3    |    4    |
|  5   |  STB0  |                   4 (Digital pin)                    |    D4    |    5    |

* Check maximum regulator / diode current to prevent a burnout when using lots of LED's. Some boards can provide only 100mA, others 800mA max.


## Example

* Examples | Erriez TM1638 | [Example](https://github.com/Erriez/ErriezTM1638/blob/master/examples/Example/Example.ino)

## Usage

**Initialization**

```c++
// Include TM1638 library
#include "TM1638.h"
  
// Connect display pins to the Arduino DIGITAL pins
#define TM1638_SCL_PIN   2
#define TM1638_DIO_PIN   3
#define TM1638_STB0_PIN  4

// Create tm1638 object
TM1638 tm1638(TM1638_SCL_PIN, TM1638_DIO_PIN, TM1638_STB0_PIN);

void setup()
{
    // Initialize TM1638
    tm1638.begin();
}
```

**Display on/off**

```c++
// Turn display off
tm1638.displayOff();
  
// Turn display on
tm1638.displayOn();
```

**Turn all LED's off**

```c++
// Turn all LED's off
tm1638.clear();
```

**Get keys**

```c++
// Get 32-bit key-scan
uint32_t keys = tm1638.getKeys();
```

**Write Byte to display register**

```c++
// Write segment LED's to the first display registers 0x00..0x0F with value 0x00..0xff to
// display numbers and characters. Just an example which depends on the hardware:
tm1638.writeData(0x01, 0x01);
```

**Write buffer to display registers**

```c++
// Creat buffer with LED's
uint8_t buf[] = { 0b10000110, 0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b00111111};

// Write buffer to TM1638
tm1638.writeData(0x00, buf, sizeof(buf));
```

## Library dependencies

- None

## Documentation

[TM1638 Datasheet](https://github.com/Erriez/ErriezTM1638/blob/master/extras/TM1638_datasheet.pdf)

## Timing

Output Benchmark example:

| Board           |  CLK   | Read keys | Write Byte | Write 16 Bytes buffer | Clear display |
| --------------- | :----: | :-------: | :--------: | :-------------------: | :-----------: |
| Pro Mini 8MHz   | 65kHz  |   736us   |   312us    |        2448us         |    2224us     |
| UNO 16MHz       | 125kHz |   340us   |   152us    |        1192us         |    1176us     |
| Node MCU 80MHz  | 200kHz |   284us   |   116us    |         683us         |     682us     |
| Node MCU 160MHz | 300kHz |   223us   |    66us    |         474us         |     469us     |

#### Arduino UNO 16MHz

![TM1638 Arduino UNO 16MHz timing](https://raw.githubusercontent.com/Erriez/ErriezTM1638/master/extras/TM1638_timing_Arduino_UNO_16MHz.png)

#### WeMos D1 & R2 80MHz

![TM1638 WeMos D1 & R2 40MHz timing](https://raw.githubusercontent.com/Erriez/ErriezTM1638/master/extras/TM1638_timing_WeMos_D1_R2_80MHz.png)

#### WeMos D1 & R2 160MHz

![TM1638 WeMos D1 & R2 160MHz timing](https://raw.githubusercontent.com/Erriez/ErriezTM1638/master/extras/TM1638_timing_WeMos_D1_R2_160MHz.png)

## Library dependencies

- The [Benchmark](https://github.com/Erriez/ErriezTM1638/blob/master/examples/Benchmark/Benchmark.ino) example uses [Erriez Timestamp](https://github.com/Erriez/ErriezTimestamp) library.

## Documentation

- [Doxygen online HTML](https://Erriez.github.io/ErriezTM1638)
- [Doxygen PDF](https://github.com/Erriez/ErriezTM1638/raw/master/docs/latex/refman.pdf)
- [TM1638 Datasheet](https://github.com/Erriez/ErriezTM1638/blob/master/extras/TM1638_datasheet.pdf)

## Library installation

Please refer to the [Wiki](https://github.com/Erriez/ErriezArduinoLibrariesAndSketches/wiki) page.