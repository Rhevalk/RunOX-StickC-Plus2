#include "app_airmouse.h"
#include <HijelHID_BLEMouse.h>

static HijelBLEMouse mouse("RunOX Mouse", "RunOX");

void app_airmouse() {
    float f_gx = 0, f_gz = 0;
    float alpha = 0.4;
    float sensitivity = 0.6;

    bool isStreaming = true;

    mouse.begin();

    while (true) {
        M5.update();

        if (M5.BtnB.wasClicked()) isStreaming = !isStreaming;

        // --- KLIK KANAN ---
        if (M5.BtnPWR.wasPressed()) mouse.press(MouseButton::Right);
        if (M5.BtnPWR.wasReleased()) mouse.release(MouseButton::Right);
        
        // --- KLIK KIRI ---
        if (M5.BtnA.wasPressed()) mouse.press(MouseButton::Left);
        if (M5.BtnA.wasReleased()) mouse.release(MouseButton::Left);  

        if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) break; 

        if (isStreaming) {
            M5.Imu.update();
            float gx, gy, gz;
            M5.Imu.getGyro(&gx, &gy, &gz);

            // Low Pass Filter (Smoothness)
            f_gx = (alpha * gx) + ((1.0 - alpha) * f_gx);
            f_gz = (alpha * gz) + ((1.0 - alpha) * f_gz);

            int dx = (int)(-f_gz * sensitivity);
            int dy = (int)(-f_gx * sensitivity);

            if (mouse.isPaired() && (dx != 0 || dy != 0)) {
                mouse.move(dx, dy);
            }

            d->print("x:");
            d->print((int)gx); 
            d->println("   ");
            d->print("z:");
            d->print((int)gz);
            d->println("   ");
            
            d->print("dx:");
            d->print(dx);
            d->println("   "); 
            d->print("dy:");
            d->print(dy);
            d->println("   ");
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    mouse.clearBonds();
    NimBLEDevice::stopAdvertising();
    NimBLEDevice::deinit(true);
}
