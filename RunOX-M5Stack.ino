#define SYSTEM_VERSION "0.6.1-beta"

#include "config.h"
#include "system.h"
#include "launcher.h"

M5GFX* d = &M5.Display;

void setup() {
    M5.begin(M5.config());

    d->setRotation(1); 
    d->setBrightness(DSP_BRIGHTNESS_LVL);

    setCpuFrequencyMhz(SYS_CPU_FREQ_MHZ);

    sys_boot_splash();
}

void loop() { 
    app_func_t selectedApp = app_launcher();
    
    if (selectedApp != nullptr) sys_execute_app(selectedApp); 
}
