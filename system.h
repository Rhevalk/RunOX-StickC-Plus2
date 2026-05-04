#pragma once

#include <M5Unified.h>
#include "config.h"

extern M5GFX* d;

void sys_boot_splash();
void sys_execute_app(void (*app_func)());
int8_t ui_picker(const char* const* options, uint8_t count);
