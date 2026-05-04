#line 1 "/home/arcshzen/Documents/Arduino/sketch/RunOX-M5Stack/clock.cpp"
#include "clock.h"

static uint32_t lastUpdate = 0;
static bool initialized = false;

void writeTwoDigits(int val) {
    d->write((val / 10) + '0'); // Digit puluhan
    d->write((val % 10) + '0'); // Digit satuan
}

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

        // --- JAM : MENIT ---
        d->setFont(&fonts::Font7);
        d->setTextColor(THM_ACCENT_FG, THM_BG);
        d->setCursor(DSP_CENTER_X - 65, DSP_CENTER_Y - 40);
        
        writeTwoDigits(dt.time.hours);
        d->write(':');
        writeTwoDigits(dt.time.minutes);

        // --- TANGGAL ---
        d->setFont(&fonts::Font2);
        d->setTextColor(THM_FG, THM_BG);
        d->setCursor(DSP_CENTER_X - 45, DSP_CENTER_Y + 30);
        
        writeTwoDigits(dt.date.date);
        d->write('-'); 
        writeTwoDigits(dt.date.month);
        d->write('-');
        
        // Untuk tahun (4 digit)
        int year = dt.date.year;
        d->print(year); // print(int) jauh lebih ringan daripada sprintf

        d->endWrite();
    }
}
