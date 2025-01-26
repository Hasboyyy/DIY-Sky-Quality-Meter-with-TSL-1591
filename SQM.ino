#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <Math.h>
#include <SPI.h>
#include <SD.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(0, 3, 4); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

const int chipSelect = D8;
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); 
// Timer variables (send new readings every one minute)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 5000; // (milliseconds)

void configureSensor(void) {
  tsl.setGain(TSL2591_GAIN_MED); // 25x gain
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS); // shortest integration time (bright light)

  Serial.println("------------------------------------");
  Serial.print("Gain: ");
  tsl2591Gain_t gain = tsl.getGain();
  switch (gain) {
    case TSL2591_GAIN_LOW:
      Serial.println("1x (Low)");
      break;
    case TSL2591_GAIN_MED:
      Serial.println("25x (Medium)");
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println("428x (High)");
      break;
    case TSL2591_GAIN_MAX:
      Serial.println("9876x (Max)");
      break;
  }

  Serial.print("Timing: ");
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(" ms");
  Serial.println("------------------------------------");
  Serial.println("");
}

void setup(void) {
  Serial.begin(115200);

  Serial.println("Starting Adafruit TSL2591 Test!");

  if (tsl.begin()) {
    Serial.println("Found a TSL2591 sensor");
  } else {
    Serial.println("No sensor found ... check your wiring?");
    while (1); // halt execution if sensor not found
  }

  configureSensor();

  Serial.println("Initializing SD card...");

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Rtc.SetDateTime(compiled);
  Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }  
}

#define countof(a) (sizeof(a) / sizeof(a[0]))
void write2SD(const RtcDateTime& dt) {

  char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );

  uint32_t lum = tsl.getFullLuminosity(); // Get CH0 & CH1 data from sensor (two 16-bit registers)
  uint16_t ir, full;
  float lux, mpsas;

  ir = lum >> 16;
  full = lum & 0xFFFF;
  
  lux = tsl.calculateLux(full, ir);
  mpsas = -2.82 * log10(lux) + 13.7;

  Serial.print("Waktu: "); Serial.println(datestring);
  Serial.print("Lux: "); Serial.println(lux); // print lux
  Serial.print("mpsas: "); Serial.println(mpsas); // print mpsas

  File dataFile = SD.open("TesMonSQU.txt", FILE_WRITE);

  // If the file is open, write lux and mpsas in CSV format
  if (dataFile) {
    Serial.println("Writing to txt...");

    // Write CSV header if it's the first time writing (optional, only for the first run)
    if (dataFile.size() == 0) {
      dataFile.println("Waktu,Lux,mpsas");  // Column headers
    }

    // Write lux and mpsas values, separated by a comma
    dataFile.print(datestring);       // Write lux value
    dataFile.print(",");
    dataFile.print(lux);       // Write lux value
    dataFile.print(",");       // Comma separator
    dataFile.println(mpsas);   // Write mpsas value
    dataFile.close();  // Close the file after writing
    Serial.println("Write complete.");
  } else {
    Serial.println("Error opening TSL.txt for writing.");
  }
}

void loop(void) {
  RtcDateTime now = Rtc.GetDateTime();
  write2SD(now);
  if (!now.IsValid())
    {
        // Common Causes:
        //    1) the battery on the device is low or even missing and the power line was disconnected
        Serial.println("RTC lost confidence in the DateTime!");
    }
  delay(timerDelay);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

