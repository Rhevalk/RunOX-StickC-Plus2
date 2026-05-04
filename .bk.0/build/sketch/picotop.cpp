#line 1 "/home/arcshzen/Documents/Arduino/sketch/RunOX-M5Stack/picotop.cpp"
#include "picotop.h"

static uint32_t lastUpdate = 0;
static bool initialized = false;

void app_picotop() {
    // Init
    if (!initialized) {
        d->setFont(&fonts::Font2);
        d->setTextColor(THM_ACCENT_FG, THM_BG);
        d->setTextPadding(d->width());

        lastUpdate = 0;
        initialized = true;
    }
    
    // Exit
    if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) {
        initialized = false; 
        load_app(app_launcher); 
        return; 
    }

if (millis() - lastUpdate > 1500 || lastUpdate == 0) {
    lastUpdate = millis();

    d->startWrite();
    
    // Ambil data (Gunakan uint32_t/uint8_t untuk menghemat register)
    uint32_t tHeap  = ESP.getHeapSize() >> 10;
    uint32_t uHeap  = tHeap - (ESP.getFreeHeap() >> 10);
    uint32_t tPsram = ESP.getPsramSize() >> 10;
    uint32_t uPsram = tPsram - (ESP.getFreePsram() >> 10);
    uint8_t cpuTemp = (uint8_t)temperatureRead();
    uint8_t cpuFreq = (uint8_t)getCpuFrequencyMhz();
    
    // VBAT: Ambil milivolt (int) saja, jangan float
    int vbat_mv = M5.Power.getBatteryVoltage(); 
    uint8_t batPct = M5.Power.getBatteryLevel();
     
    // SRAM
    d->setCursor(8, 8);
    d->write(("SRAM  : ")); d->print(uHeap); d->write((" / ")); d->print(tHeap); d->write((" KB"));
    
    // PSRAM
    d->setCursor(8, 28);
    d->write(("PSRAM : ")); d->print(uPsram); d->write((" / ")); d->print(tPsram); d->write((" KB"));
    
    // CPU
    d->setCursor(8, 48);
    d->write(("CPU   : ")); d->print(cpuTemp); d->write((" C (")); d->print(cpuFreq); d->write((" MHz)"));
    
    // Battery - MANUAL FLOAT (Suckless Style)
    d->setCursor(8, 68);
    d->write(("BAT   : "));
    d->print(vbat_mv / 1000); // Volt (Integer)
    d->write('.');
    int fraction = (vbat_mv % 1000) / 10; // Ambil 2 digit belakang koma
    if (fraction < 10) d->write('0');
    d->print(fraction);
    
    d->write((" V (")); d->print(batPct); d->write((" %)"));

    d->endWrite();
}
}
