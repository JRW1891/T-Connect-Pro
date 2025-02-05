/*
 * @Description: None
 * @version: V1.0.0
 * @Author: None
 * @Date: 2024-07-08 16:34:12
 * @LastEditors: LILYGO_L
 * @LastEditTime: 2024-07-08 16:45:18
 * @License: GPL 3.0
 */

#include <Arduino.h>
#include "pin_config.h"

static uint8_t i = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    pinMode(RELAY_1, OUTPUT);
    digitalWrite(RELAY_1, i);
}

void loop()
{
    digitalWrite(RELAY_1, i);

    i = !i;
    delay(3000);
}