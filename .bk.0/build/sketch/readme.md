#line 1 "/home/arcshzen/Documents/Arduino/sketch/RunOX-M5Stack/readme.md"
“No animations. No background tasks. No wasted CPU. Every cycle is predictable.”
“If it doesn’t need to run, it doesn’t exist

# Apps:
    - Clock (RTC BM8563)
        >> view
        >> Settings
        >> alarm
    
    - Air Mouse (BLE HID + MPU6886)
    - Presenter (BLE HID + Button)
    - picotop
        >> SRAM
        >> PSRAM
        >> CPU Temp
        >> CPU Freq
        >> Bat Volt
        >> Bat Pers
    
    - Sound Record (SPM1423-PDM MIC)
    - IR Tx
    - Voice Recognition
    - Settings
        >> Brightness
        >> CPU Freq

# Button Func
Click (A)      : select
Click (B | C)  : navigation

Hold A 1s      : opt 1
Hold A 2s      : opt 2

Hold B 1s      : opt 1
Hold B 2s      : Exit Application

Hold C 1s      : opt 1
Hold C 2s      : opt 2
Hold C 6s      : Shutdown

# Static Application Format 
void app_name() {
    // INIT 
    if (!initialized) {
        // setup awal (font, state, CPU freq, dll)
        initialized = true;
    }

    // EXIT 
    if (M5.BtnB.pressedFor(2000)) {
        initialized = false;
        load_app(app_launcher);
        return;
    }

    // LOOP (update berkala)
    if (millis() - lastUpdate > INTERVAL || lastUpdate == 0) {
        lastUpdate = millis();

        d->startWrite();

        // render UI / logic di sini

        d->endWrite();
    }
}

# File Struckture   
    - RunOX-M5Stack.ino
    └──> [Kernel yang berisikan konfigurasi awal system dan yang mengatur app berjalan]

    - config.h
    └──> [Semua deklarasi variabel tetap]
    - system.h
    └──> [Header untuk jembatan antara kernel dengan app]

    - entries.x
    └──> [Kumpulan nama dan fungsi pada app]
    - modules.h
    └──> [Kumpulan #Include header app]

    - launcher.cpp
    └──> [App menu yang digunakan untuk memilih aplikasi]
    - launcher.h
    └──> [Header menu luncher]

    clock.cpp
    └──> [App]
    - clock.h
    └──> [Header App]

    - picotop.cpp
    └──> [APP]
    - picotop.h
    └──> [APP]

