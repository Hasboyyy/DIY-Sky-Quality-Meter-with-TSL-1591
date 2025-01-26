# TSL2591 Data Logger

This project is a light intensity data logging system using the TSL2591 sensor, an RTC DS1302 module for timestamping, and an SD card module for data storage. The data is logged in CSV format, making it easy to access and analyze.

## Features
- Measures light intensity in Lux and mpsas (magnitudes per square arcsecond).
- Logs data with accurate timestamps using the DS1302 RTC.
- Stores data in CSV format on an SD card.
- Supports automatic logging at specified intervals.

## Hardware Requirements
- **Microcontroller**: Arduino Nano.
- **Light Sensor**: Adafruit TSL2591.
- **RTC Module**: DS1302.
- **SD Card Module**.
- Necessary cables and connectors.

## Software Requirements
- **Arduino IDE** (latest version).
- Required libraries:
  - [`Adafruit_Sensor`](https://github.com/adafruit/Adafruit_Sensor)
  - [`Adafruit_TSL2591`](https://github.com/adafruit/Adafruit_TSL2591_Library)
  - [`ThreeWire`](https://github.com/PaulStoffregen/ThreeWire)
  - [`RtcDS1302`](https://github.com/Makuna/Rtc)
  - Arduino built-in libraries for SPI and SD.

## Wiring Diagram
Connect the components as per the table below:

| Component           | Arduino Pin | Component Pin       |
|---------------------|-------------|---------------------|
| TSL2591             | I2C (A4/A5)| SDA/SCL             |
| RTC DS1302          | Pin 0, 3, 4| IO, SCLK, CE        |
| SD Card Module      | D8          | Chip Select (CS)    |

## How to Use
1. **Download Libraries**: Ensure all the libraries mentioned above are installed in the Arduino IDE.
2. **Upload Code**: Upload the `TSL2591_Logger.ino` file to your Arduino board using the Arduino IDE.
3. **Insert SD Card**: Place an SD card into the SD card module.
4. **Power Up**: Connect the microcontroller to a power source.
5. **Check Serial Monitor**: Open the Serial Monitor in the Arduino IDE (baud rate: 115200) to verify the system is running.
6. **Retrieve Data**: Remove the SD card and open the CSV file on your computer to view the logged data.
