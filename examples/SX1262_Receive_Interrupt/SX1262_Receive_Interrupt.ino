/*
 * @Description: None
 * @Author: LILYGO_L
 * @Date: 2024-07-25 17:05:35
 * @LastEditTime: 2024-07-25 17:33:10
 * @License: GPL 3.0
 */

#include <RadioLib.h>
#include "pin_config.h"

SX1262 radio = new Module(SX1262_CS, SX1262_DIO1, SX1262_RST, SX1262_BUSY, SPI);

void setup()
{
    Serial.begin(115200);

    // initialize SX1262 with default settings
    Serial.print("[SX1262] Initializing ... ");
    SPI.begin(SX1262_SCLK, SX1262_MISO, SX1262_MOSI);
    // int state = radio.beginFSK();
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("success!");
    }
    else
    {
        Serial.print("failed, code ");
        Serial.println(state);
        while (true)
            ;
    }

    radio.setFrequency(872.1);
    radio.setBandwidth(500);
    radio.setSpreadingFactor(9);
    radio.setCodingRate(6);
    radio.setSyncWord(0xAB);
    radio.setOutputPower(22);
    radio.setPreambleLength(16);
    radio.setCRC(false);

    // set the function that will be called
    // when new packet is received
    radio.setPacketReceivedAction(setFlag);

    // start listening for LoRa packets
    Serial.print("[SX1262] Starting to listen ... ");
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("success!");
    }
    else
    {
        Serial.print("failed, code ");
        Serial.println(state);
        while (true)
            ;
    }

    // if needed, 'listen' mode can be disabled by calling
    // any of the following methods:
    //
    // radio.standby()
    // radio.sleep()
    // radio.transmit();
    // radio.receive();
    // radio.scanChannel();
}

volatile bool receivedFlag = false;

void setFlag(void)
{
    // we got a packet, set the flag
    receivedFlag = true;
}

void loop()
{
    // check if the flag is set
    if (receivedFlag)
    {
        // reset flag
        receivedFlag = false;

        // you can read received data as an Arduino String
        String str;
        int state = radio.readData(str);

        // you can also read received data as byte array
        /*
          byte byteArr[8];
          int numBytes = radio.getPacketLength();
          int state = radio.readData(byteArr, numBytes);
        */

        if (state == RADIOLIB_ERR_NONE)
        {
            // packet was successfully received
            Serial.println("[SX1262] Received packet!");

            // print data of the packet
            Serial.print("[SX1262] Data:\t\t");
            Serial.println(str);

            // print RSSI (Received Signal Strength Indicator)
            Serial.print("[SX1262] RSSI:\t\t");
            Serial.print(radio.getRSSI());
            Serial.println(" dBm");

            // print SNR (Signal-to-Noise Ratio)
            Serial.print("[SX1262] SNR:\t\t");
            Serial.print(radio.getSNR());
            Serial.println(" dB");

            // print frequency error
            Serial.print("[SX1262] Frequency error:\t");
            Serial.print(radio.getFrequencyError());
            Serial.println(" Hz");

            Serial.print("[SX1262] DataRate:\t\t");
            Serial.print(radio.getDataRate());
            Serial.println(" bps");
        }
        else if (state == RADIOLIB_ERR_CRC_MISMATCH)
        {
            // packet was received, but is malformed
            Serial.println("CRC error!");
        }
        else
        {
            // some other error occurred
            Serial.print("failed, code ");
            Serial.println(state);
        }
    }
}
