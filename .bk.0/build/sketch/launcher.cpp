#line 1 "/home/arcshzen/Documents/Arduino/sketch/RunOX-M5Stack/launcher.cpp"
#include "launcher.h"

constexpr uint8_t itemCount = sizeof(apps) / sizeof(apps[0]);

int8_t currentIndex = 0;
bool redraw = true;

void app_launcher() {
    auto* d = &M5.Display; 

    if (M5.BtnB.wasClicked()) {
        currentIndex = (currentIndex + 1) % itemCount;
        redraw = true;
    }

    if (M5.BtnPWR.wasClicked()) {
        currentIndex = (currentIndex - 1 + itemCount) % itemCount;
        redraw = true;
    }

    if (M5.BtnA.wasClicked()) {
        redraw = true;
        load_app(apps[currentIndex].func);
        return;
    }

    if (redraw) {  
        d->startWrite();

        d->fillRect(8, 0, 2, d->height(), THM_BG);
        d->setFont(&fonts::Font2);
        d->setTextDatum(top_left);
    
        constexpr uint8_t VISIBLE_ITEMS = 6;
        constexpr uint8_t ITEM_HEIGHT = 22;
    
        uint8_t startIdx = (currentIndex > 2) ? (currentIndex - 2) : 0;
        uint8_t maxStart = (itemCount > VISIBLE_ITEMS) ? (itemCount - VISIBLE_ITEMS) : 0;
        if (startIdx > maxStart) startIdx = maxStart;
    
        for (uint8_t i = 0; i < VISIBLE_ITEMS; i++) {
            uint8_t menuIdx = startIdx + i;
            if (menuIdx >= itemCount) break;
    
            uint8_t yPos = 10 + (i * ITEM_HEIGHT);
            
            if (menuIdx == currentIndex) {
                d->drawFastVLine(8, yPos - 2, ITEM_HEIGHT - 2, THM_ACCENT_FG);
                d->drawFastVLine(9, yPos - 2, ITEM_HEIGHT - 2, THM_ACCENT_FG);
            }
    
            d->setTextColor(THM_FG, THM_BG);
            d->setCursor(20, yPos);
            d->write(apps[menuIdx].name);
        }
    
        d->endWrite();
        redraw = false;
    }
}
