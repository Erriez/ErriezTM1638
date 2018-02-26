# TM1638 library for Arduino

This is a 3-pin serial TM1638 chip library for Arduino. It supports a combined 
LED driver controller and key-scan interface.

![TM1638 chip](https://raw.githubusercontent.com/Erriez/ErriezTM1638/master/extras/TM1638_pins.jpg)

## Hardware

Connect power and 3 data pins to an Arduino board DIGITAL pins:
* VDD (Power 5V +/- 10%)
* GND (Ground)
* DIO (Bi-directional data input/output)
* STB (Chip select)
* CLK (Clock)

The following TM1638 pins should be connected to LED's and buttons in a matrix:  
* K1~K3 (Key-scan data input)
* SEG/GRID (Output for LED matrix)



## Documentation

[TM1638 Datasheet](https://github.com/Erriez/ErriezTM1638/blob/master/extras/TM1638_datasheet.pdf)



## Example

Examples | TM1638 | [Example](https://github.com/Erriez/ErriezTM1638/blob/master/examples/Example/Example.ino)



## Usage

**Initialization**

```c++
// Include TM1638 library
#include "TM1638.h"
  
// Connect display pins to the Arduino DIGITAL pins
#define DIO_PIN   2
#define SCL_PIN   3
#define STB_PIN   4
  
// Create TM1638 object
TM1638 tm1638(DIO_PIN, SCL_PIN, STB_PIN);
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



**Get key-scan**

```c++
// Get 32-bit key-scan
uint32_t keys = tm1638.getKeyScan();
```



**Write display register**

```c++
// Write segment LED's to the first display register
// The LED's turned on depends on your hardware SEG/GRID connections
// Experiment with the registers 0x00..0x0F value 0x00..0xff to display numbers
// and characters, for example:
tm1638.writeDisplayRegister(0x01, 0x01);
```



## Library dependencies
* None
