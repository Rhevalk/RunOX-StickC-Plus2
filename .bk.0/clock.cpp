#include "clock.h"

static uint32_t lastUpdate = 0;
static bool initialized = false;

void app_clock() {
    // Init
    if (!initialized) {
        setCpuFrequencyMhz(20);

        lastUpdate = 0;
        initialized = true;
    }
    
    // Exit
    if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) {
        setCpuFrequencyMhz(SYS_CPU_FREQ_MHZ);
        initialized = false; 
        
        load_app(app_launcher); 
        return; 
    }

    if (millis() - lastUpdate > 30000 || lastUpdate == 0) {
        lastUpdate = millis();
    
        auto dt = M5.Rtc.getDateTime();
    
        d->startWrite();
    
        char buffer[16];
    
        d->setFont(&fonts::Font7);
        d->setTextDatum(middle_center);
        d->setTextColor(THM_ACCENT_FG, THM_ACCENT_BG);
    
        sprintf(buffer, "%02d:%02d", dt.time.hours, dt.time.minutes);
        d->drawString(buffer, DSP_CENTER_X - 10, DSP_CENTER_Y - 20);
    
        d->setFont(&fonts::Font2);
        d->setTextColor(THM_FG, THM_BG);
    
        sprintf(buffer, "%02d-%02d-%04d", dt.date.date, dt.date.month, dt.date.year);
        d->drawString(buffer, DSP_CENTER_X, DSP_CENTER_Y + 30);
    
        d->endWrite();
    }
}
