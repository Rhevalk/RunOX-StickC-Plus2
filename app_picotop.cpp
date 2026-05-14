#include "app_picotop.h"

void app_picotop() {
    // Init
    uint32_t lastUpdate = 0;

    d->setFont(&fonts::Font2);
    d->setTextColor(THM_ACCENT_FG, THM_BG);
    d->setTextPadding(d->width());
    
    while (true) {
        M5.update();

        // Exit
        if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) break; 

        if (millis() - lastUpdate > 1500 || lastUpdate == 0) {
            lastUpdate = millis();
        
            d->startWrite();
            
            uint32_t tHeap  = ESP.getHeapSize() >> 10;
            uint32_t uHeap  = tHeap - (ESP.getFreeHeap() >> 10);
            
            uint32_t tPsram = ESP.getPsramSize() >> 10;
            uint32_t uPsram = tPsram - (ESP.getFreePsram() >> 10);
            
            int      cpuTemp = (int)temperatureRead();
            int      cpuFreq = getCpuFrequencyMhz();
            
            float    vbat   = M5.Power.getBatteryVoltage() * 0.001f;
            int      batPct  = M5.Power.getBatteryLevel();

            // SRAM
            d->setCursor(8, 8);
            d->print(F("[SRM] ")); d->print(uHeap); d->print(F(" / ")); d->print(tHeap); d->print(F(" KB"));
            
            // PSRAM
            d->setCursor(8, 28);
            d->print(F("[PSR] ")); d->print(uPsram); d->print(F(" / ")); d->print(tPsram); d->print(F(" KB"));
            
            // CPU
            d->setCursor(8, 48);
            d->print(F("[CPU] ")); d->print(cpuTemp); d->print(F(" C (")); d->print(cpuFreq); d->print(F(" MHz)"));
            
            // Battery
            d->setCursor(8, 68);
            d->print(F("[BAT] ")); d->print(vbat, 2); d->print(F(" V (")); d->print(batPct); d->print(F(" %)"));
            
            d->endWrite();
        } 
        vTaskDelay(pdMS_TO_TICKS(SYS_LOOP_INTERVAL_MS));
    }
}
