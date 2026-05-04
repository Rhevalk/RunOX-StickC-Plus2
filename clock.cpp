#include "clock.h"

void app_clock() {
    // Init
    bool isOn = true;
    bool forceOn = false;
    float fAz = 0, fAx = 0;
    uint32_t lastUpdate = 0;

    while (true) {
        M5.update();    

        // Exit
        if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) break; 

        if (M5.BtnB.wasClicked()) { forceOn = !forceOn; isOn = true; d->setBrightness(DSP_BRIGHTNESS_LVL); lastUpdate = 0; }

        if (!forceOn) {
            M5.Imu.update();
            float ax, ay, az; M5.Imu.getAccelData(&ax, &ay, &az);            
            
            fAz = (0.2f * az) + (0.8f * fAz); fAx = (0.2f * ax) + (0.8f * fAx);
        
            if (!isOn && fAz >= 0.8f && abs(fAx) >= 0.3f) {
                isOn = true; d->setBrightness(DSP_BRIGHTNESS_LVL); 
            } 
            else if (isOn && (fAz <= 0.4f || abs(fAx) <= 0.05f)) {
                isOn = false; d->setBrightness(0); 
            }
        }

        if (isOn && millis() - lastUpdate > 30000 || lastUpdate == 0) {
            lastUpdate = millis();
        
            auto dt = M5.Rtc.getDateTime();
        
            d->startWrite();
        
            char buffer[16];
        
            d->setFont(&fonts::Font7);
            d->setTextDatum(middle_center);
            d->setTextColor(THM_ACCENT_FG, THM_ACCENT_BG);
        
            sprintf(buffer, "%02d:%02d", dt.time.hours, dt.time.minutes);
            d->drawString(buffer, DSP_CENTER_X, DSP_CENTER_Y - 20);
        
            d->setFont(&fonts::Font2);
            d->setTextColor(THM_FG, THM_BG);
        
            sprintf(buffer, "%02d-%02d-%04d", dt.date.date, dt.date.month, dt.date.year);
            d->drawString(buffer, DSP_CENTER_X, DSP_CENTER_Y + 30);
        
            d->endWrite();
        }
        vTaskDelay(pdMS_TO_TICKS(SYS_LOOP_INTERVAL_MS));
    }
}
