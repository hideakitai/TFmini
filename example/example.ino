#include "TFmini.h"

HardwareSerial Serial1(1);
HardwareSerial Serial2(2);
TFmini tfmini[2];

void setup()
{
    Serial.begin(115200);
    Serial1.begin(TFmini::DEFAULT_BAUDRATE, SERIAL_8N1, 26, 27);
    Serial2.begin(TFmini::DEFAULT_BAUDRATE);
    tfmini[0].attach(Serial1);
    tfmini[1].attach(Serial2);
}

void loop()
{
    for (uint8_t i = 0; i < 2; ++i)
    {
        if (tfmini[i].available())
        {
            Serial.print("tfmini ");
            Serial.println(i);
            Serial.print("distance : ");
            Serial.println(tfmini[i].getDistance());
            Serial.print("strength : ");
            Serial.println(tfmini[i].getStrength());
            Serial.print("int time : ");
            Serial.println(tfmini[i].getIntegrationTime());
        }
    }
}
