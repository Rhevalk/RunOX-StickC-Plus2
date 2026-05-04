#pragma once

#include <M5Unified.h>

extern M5GFX* d;

void load_app(void (*app_func)());

void app_launcher();
