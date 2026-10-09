#include "config.h"
#include "system.h"
#include "modules.h"

M5GFX* d = &M5.Display;

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

app_func_t launcher() {
    int choice = ui_picker(appNames, itemCount);
    if (choice >= 0 && choice < itemCount) return apps[choice].func; 
    return nullptr;
}

void setup() {
    M5.begin(M5.config());

    d->setRotation(1); 
    d->setBrightness(DSP_BRIGHTNESS_LVL);

    setCpuFrequencyMhz(SYS_CPU_FREQ_MHZ);

    sys_boot_splash();
}

void loop() { 
    app_func_t selectedApp = launcher();
    if (selectedApp != nullptr) sys_execute_app(selectedApp); 
}
