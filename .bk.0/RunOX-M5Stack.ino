#define SYSTEM_VERSION "0.9-alpha"

#include <M5Unified.h>
#include "config.h"
#include "system.h"

uint32_t lastActivity = 0;
bool screenOff = false;

M5GFX* d = &M5.Display;

void (*current_app)(); 

void load_app(void (*app_func)()) {
    if (app_func == nullptr) return;

    d->fillScreen(BLACK); 
    current_app = app_func;
}

void setup() {
    M5.begin(M5.config());

    setCpuFrequencyMhz(SYS_CPU_FREQ_MHZ);

    d->setBrightness(DSP_BRIGHTNESS_LVL);
    d->setRotation(1); 

    load_app(app_launcher); 
}

void loop() {
    M5.update();
    
    if (M5.BtnA.wasClicked() || M5.BtnB.wasClicked() || M5.BtnPWR.wasClicked()) {
        lastActivity = millis();
        if (screenOff) {
            M5.Display.setBrightness(DSP_BRIGHTNESS_LVL);
            screenOff = false;
            return;
        }
    }

    // idle check
    if (!screenOff && millis() - lastActivity > SYS_IDLE_TIMEOUT_MS) {
        M5.Display.setBrightness(0);
        screenOff = true;
    }

    if (current_app != nullptr) current_app();

    delay(SYS_LOOP_INTERVAL_MS);
}
