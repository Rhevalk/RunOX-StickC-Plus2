#include "app_irremote.h"

static IRsend irsend(IR_PIN);

void app_irremote() {
    irsend.begin();

    while(true) {
        M5.update();

        // Exit
        if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) break; 

        if (M5.BtnA.wasPressed()) {         
            irsend.sendNEC(POWER_CODE);
        }
        vTaskDelay(pdMS_TO_TICKS(SYS_LOOP_INTERVAL_MS));
    }
}
