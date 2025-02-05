/*
 * @Description: None
 * @version: V1.0.0
 * @Author: None
 * @Date: 2023-07-15 17:35:13
 * @LastEditors: LILYGO_L
 * @LastEditTime: 2024-03-29 14:45:35
 * @License: GPL 3.0
 */

#include <SPI.h>
#include <Ethernet.h>
#include "pin_config.h"
#include "utility/w5100.h"

void EthernetReset(const uint8_t resetPin)
{
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(250);
    digitalWrite(resetPin, LOW);
    delay(50);
    digitalWrite(resetPin, HIGH);
    delay(350);
}

void setup()
{
    Serial.begin(115200);

    SPI.begin(W5500_SCLK, W5500_MISO, W5500_MOSI, W5500_CS); // SPI boots
    EthernetReset(W5500_RST);
    Ethernet.init(W5500_CS);

    W5100.init();
    delay(1000); // give the Ethernet shield a second to initialize
}

void loop()
{
    switch (Ethernet.hardwareStatus())
    {
    case EthernetNoHardware:
        Serial.println("Ethernet No Hardware");
        break;
    case EthernetW5100:
        Serial.println("Ethernet W5100");
        break;
    case EthernetW5200:
        Serial.println("Ethernet W5200");
        break;
    case EthernetW5500:
        Serial.println("Ethernet W5500");
        break;
    }

    switch (Ethernet.linkStatus())
    {
    case Unknown:
        Serial.print("Link status: ");
        Serial.println("Unknown");
        break;
    case LinkON:
        Serial.print("Link status: ");
        Serial.println("ON");
        break;
    case LinkOFF:
        Serial.print("Link status: ");
        Serial.println("OFF");
        break;
    }

    Serial.println("");
    delay(1000);
}
