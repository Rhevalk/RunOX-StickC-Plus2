#include "system.h"

void sys_boot_splash() {
    d->drawBitmap(56, 49, RunOX, 128, 36, WHITE, BLACK); 
    delay(2000);
}

void sys_execute_app(void (*app_func)()) {
    d->clear();
    
    app_func(); 

    d->setBrightness(DSP_BRIGHTNESS_LVL);
    setCpuFrequencyMhz(SYS_CPU_FREQ_MHZ);

    d->clear();
}

/**
 * @brief Fungsi universal untuk memilih item dari daftar (mendukung scrolling).
 * @return int8_t Index yang dipilih (0 to count-1), atau -2 jika user menekan BACK.
 */
int ui_picker(const char* const* options, int count) {
    int currentIndex = 0;
    bool redraw = true;
    
    constexpr int TOP_MARGIN = 10;
    constexpr int VISIBLE_ITEMS = 6;
    constexpr int ITEM_HEIGHT = 20;

    LGFX_Sprite canvas(d); 
    canvas.setPsram(true); 
    if (canvas.createSprite(d->width(), d->height()) == nullptr) {
        canvas.setPsram(false);
        canvas.createSprite(d->width(), d->height());
    }

    canvas.setFont(&fonts::Font2);

    while (true) {
        M5.update();
        
        if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) return -1;
        
        if (M5.BtnA.wasClicked()) return currentIndex;

        if (M5.BtnPWR.wasClicked()) { 
            currentIndex = (currentIndex + 1) % count; 
            redraw = true; 
        }
        if (M5.BtnB.wasClicked()) { 
            currentIndex = (currentIndex - 1 + count) % count; 
            redraw = true; 
        }

        if (redraw) { 
            canvas.fillScreen(THM_BG); 
        
            int startIdx = (currentIndex > 2) ? (currentIndex - 2) : 0;
            int maxStart = (count > VISIBLE_ITEMS) ? (count - VISIBLE_ITEMS) : 0;
            if (startIdx > maxStart) startIdx = maxStart;
        
            for (int i = 0; i < VISIBLE_ITEMS; i++) {
                int menuIdx = startIdx + i;
                if (menuIdx >= count) break;
                int yPos = TOP_MARGIN + (i * ITEM_HEIGHT) + 2;

                if (menuIdx == currentIndex) {
                    canvas.drawFastVLine(8, yPos, ITEM_HEIGHT - 2, THM_ACCENT_FG);
                    canvas.drawFastVLine(9, yPos, ITEM_HEIGHT - 2, THM_ACCENT_FG);
                    canvas.setTextColor(THM_ACCENT_FG, THM_BG);
                } else {
                    canvas.setTextColor(THM_FG, THM_BG);
                }
                canvas.setCursor(20, yPos);
                canvas.print(options[menuIdx]);
            }
        
            canvas.pushSprite(0, 0); 
            redraw = false;
        }
        vTaskDelay(pdMS_TO_TICKS(SYS_LOOP_INTERVAL_MS));
    }
}
