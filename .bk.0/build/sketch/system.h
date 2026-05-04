#line 1 "/home/arcshzen/Documents/Arduino/sketch/RunOX-M5Stack/system.h"
#pragma once

#include <M5Unified.h>

extern M5GFX* d;

void load_app(void (*app_func)());

void app_launcher();
