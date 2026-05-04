#pragma once

#include <M5Unified.h>
#include "modules.h"
#include "config.h"
#include "system.h"

struct App { const char* name; void (*func)(); };

const App apps[] = {
    #define APP_ENTRY(name, func) {name, func},
    #include "entries.x"
    #undef APP_ENTRY
};
