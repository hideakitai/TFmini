# TFmini

Arduino library for [Benewake TFmini micro LiDAR module](http://benewake.com/en/tfmini.html)

## Usage

```C++
#include "TFmini.h"

TFmini tfmini;

void setup() {
    Serial.begin(115200);
    Serial1.begin(TFmini::DEFAULT_BAUDRATE);
    tfmini.attach(Serial1);
}

void loop() {
    if (tfmini.available()) {
        Serial.print("distance : ");
        Serial.println(tfmini.getDistance());
        Serial.print("strength : ");
        Serial.println(tfmini.getStrength());
        Serial.print("int time : ");
        Serial.println(tfmini.getIntegrationTime());
    }
}
```

### Supported Configuration

```C++
// default : Standard
void setOutputDataFormat(const OutputDataFormat format);

// default : 10ms
void setOutputDataPeriod(const uint16_t ms);

// default : cm
void setOutputDataUnit(const OutputDataUnit unit);

// default : Auto
void setDetectionPattern(const DetectionPattern pttr);

// usable when detection pattern is Fixed
void setDistanceMode(const DistanceMode mode);

// default : 12m
void setRangeLimit(uint16_t mm);
void disableRangeLimit();

// default : low = 20(DEC), high & cm is undefined
void setSignalStrengthThreshold(uint8_t low, uint16_t high, uint8_t cm);

// default : 115200 (0x06)
void setBaudRate(Baudrate baud);

// default : Internal (100Hz)
void setTriggerSource(const TriggerSource trigger);

// reset all settings
void resetSettings();
```

### Public Constnts

```C++
static const uint32_t DEFAULT_BAUDRATE = 115200;

enum class OutputDataFormat { Standard = 0x01, Pixhawk = 0x04 };
enum class OutputDataUnit { MM = 0x00, CM = 0x01 };
enum class DetectionPattern { Auto = 0x00, Fixed = 0x01 };
enum class DistanceMode { Short = 0x02, Long = 0x07 };
enum class TriggerSource { Internal = 0x01, External = 0x00 };
enum class Baudrate
{
    BAUD_9600, BAUD_14400, BAUD_19200, BAUD_38400, BAUD_56000, BAUD_57600, BAUD_115200,
    BAUD_128000, BAUD_230400, BAUD_256000, BAUD_460800, BAUD_500000, BAUD_512000
};
```

## Note

Pixhawk data format is not supported yet

## License

MIT
