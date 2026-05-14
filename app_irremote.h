#pragma once

#define IR_PIN 19
#define POWER_CODE 0xC12FE817

#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "config.h"
#include "system.h"

void app_irremote();
