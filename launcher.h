#pragma once

#include "modules.h"
#include "config.h"
#include "system.h"

typedef void (*app_func_t)();

struct App { 
  const char* name; 
  void (*func)(); 
};

const App apps[] __attribute__((aligned(4))) = {
    #define APP_ENTRY(name, func) {name, func},
    #include "entries.x"
    #undef APP_ENTRY
};

const char* const appNames[] = {
    #define APP_ENTRY(name, func) name,
    #include "entries.x"
    #undef APP_ENTRY
};

constexpr int itemCount = sizeof(appNames) / sizeof(appNames[0]);

app_func_t launcher();   
