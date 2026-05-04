#include "air_keys.h"
#include <HijelHID_BLEKeyboard.h>

static HijelHID_BLEKeyboard keyboard("RunOX Keys", "RunOX");

void app_airkeys() {
    bool lastStatus = false;

    const char* modeOptions[] = {"Presenter", "Audio Player"};
    
    int8_t modeIndex = ui_picker(modeOptions, 2);
    if (modeIndex == -1) return;

    keyboard.begin();

    while (true) {    
        M5.update();
        
        if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) break;    

        bool currentStatus = keyboard.isPaired();

        if (currentStatus != lastStatus) {
            d->startWrite();
            d->fillScreen(THM_BG);
            d->setCursor(20, 10);
            if (currentStatus) {
                d->setTextColor(GREEN);
                d->printf("Online: %s", modeOptions[modeIndex]);
            } else {
                d->setTextColor(RED);
                d->println("Offline");
            }
            d->endWrite();
            lastStatus = currentStatus;
        }

        if (currentStatus) {
            if (modeIndex == 0) { // PRESENTER MODE
                if (M5.BtnB.wasClicked())    keyboard.tap(KEY_RIGHT);
                if (M5.BtnPWR.wasClicked())  keyboard.tap(KEY_LEFT);
                if (M5.BtnA.wasClicked())    keyboard.tap(KEY_RETURN); 
            } 
            else { // AUDIO PLAYER MODE
                if (M5.BtnB.wasClicked())    keyboard.tap(MEDIA_NEXT_TRACK);
                if (M5.BtnPWR.wasClicked())  keyboard.tap(MEDIA_PREV_TRACK);
                if (M5.BtnA.wasClicked())    keyboard.tap(MEDIA_PLAY_PAUSE);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(SYS_LOOP_INTERVAL_MS));
    }
    
    keyboard.end(); 
}
