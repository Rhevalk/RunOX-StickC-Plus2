CATATAN:
tommbol pwr tidak akan otomatis mematikan sistem, dia perlu kembli ke launcher dan memasuki powr menu

IR Power  NEC2  C12FE817
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

# Button Func
Click (A)      : select
Click (B | C)  : navigation

Hold A 1s      : opt 1
Hold A 2s      : opt 2

Hold B 1s      : opt 1
Hold B 2s      : Exit App

Hold C 1s      : opt 1
Hold C 6s      : Power Menu

# RunOX-M5Stick Plus 2

> **"No animations. No background tasks. No wasted CPU. Every cycle is predictable."**
> 

**RunOX (Runtime Operating Executive)** adalah *minimalist operating executive* yang dirancang khusus untuk **M5StickC Plus2**. Berjalan di atas **FreeRTOS** dengan abstraksi **M5Unified**, sistem ini mengusung konsep *single-app execution* dengn tujuan menghilangkan *multitasking* di lapisan dasar demi mencapai efisiensi ekstrem, stabilitas tinggi, dan kontrol penuh tanpa beban *overhead*

---

Sistem RunOX memungkinkan Anda untuk mengganti **Launcher** secara *custom* dan melakukan lepas-pasang (**Plug-and-Play**) modul **Apps**. Namun, **Base** bersifat statis.

*Untuk melakukan modifikasi pada Base, Anda diwajibkan menyesuaikan ulang struktur Launcher dan format standar Apps yang digunakan.*

---

## Kebutuhan

RunOX menggunakan pustaka `<**M5Unified**>` jadi anda perlu menginstall pustaka tersebut dan pastikan ana sudah menginstall Board ESP32

### 1. Menggunakan Arduino IDE (Graphical Interface)

1. **Library Manager**:
    - Buka Arduino IDE, pergi ke **Sketch** > **Include Library** > **Manage Libraries...**
    - Cari dan instal **"M5Unified"** oleh M5Stack.
    - (Opsi tambahan) Instal **"M5GFX"** jika belum terinstal otomatis sebagai dependensi.
2. **Konfigurasi Board**:
    - Pilih **Tools** > **Board** > **ESP32 Arduino** > **M5StickC Plus2**.
    - Atur *Upload Speed* ke `1500000` untuk proses flashing yang cepat.
3. **Kompilasi**:
    - Buka file `RunOX-M5Stack.ino`.
    - Klik tombol **Verify**, lalu **Upload**.

### 3. Menggunakan Arduino CLI (Command Line Interface)

Untuk workflow yang lebih "Suckless" dan otomatis, gunakan perintah berikut di terminal Anda:

**A. Update Index & Instal Board Core**

```bash
arduino-cli core update-index
arduino-cli core install esp32:esp32
```

**B. Instal Library Dependensi**

```bash
arduino-cli lib install "M5Unified"
arduino-cli lib install "M5GFX"

# Menginstall M5Unified akan otomatis menginstall M5GFX, jadi baris kedua sifatnya opsional untuk dilakukan
```

**C. Kompilasi (Compile)**
Gunakan perintah ini di dalam direktori proyek RunOX:

```bash
arduino-cli compile --fqbn esp32:esp32:m5stick_c_plus2 .
```

**D. Unggah (Upload)**
Ganti `PORT` dengan alamat port perangkat Anda (misal: `/dev/ttyUSB0` atau `/dev/ttyACM0` atau `COM3`):

```bash
arduino-cli upload -p PORT --fqbn esp32:esp32:m5stick_c_plus2 .
```

## Arsitektur Sistem (Suckless Style)

RunOX membagi tanggung jawab sistem ke dalam tiga lapisan modular yang kaku namun efisien:

**1. The Base (Hardware Abstraction Layer)**
Dibangun di atas **Arduino ESP32 Framework** dan **M5Unified**. Base berfungsi sebagai fondasi statis yang menangani komunikasi *low-level* dengan periferal. Base bersifat *immutable* bagi lapisan aplikasi untuk menjaga integritas sistem.

**2. The Launcher (System Gateway)**
Aplikasi manajer yang berfungsi sebagai *entry point*. Launcher dipanggil secara otomatis oleh Base saat *cold boot* atau ketika sebuah aplikasi melepaskan kendali (*exit*).

**3. The Application (User Space)**
Lapisan tertinggi dengan kontrol penuh. Developer memiliki akses mutlak ke siklus CPU dan perangkat keras. **Catatan:** Manajemen sumber daya (SRAM, PSRAM, daya, dan periferal) adalah tanggung jawab penuh pengembang aplikasi.

> **Catatan:** Tanggung jawab manajemen sumber daya (memori, periferal, daya) sepenuhnya dilepas kepada pembuat lapisan Aplikasi.
> 

---

## Struktur File

RunOX menggunakan pola modular untuk manajemen memori yang efisien dan interkoneksi antar *app-module* yang bersih.

- **`RunOX-M5Stack.ino`**: *Executive Entry Point*. Menangani inisialisasi hardware (`M5.begin()`) dan logika *main app switcher*.
- **`config.h`**: *Single source of truth* untuk definisi konstanta global (Clock Speed, Intervals, Pinout).
- **`system.h`**: Interface jembatan (*header*) agar aplikasi dapat mengakses fungsi global dan API sistem. Deklarasi Luncher diletakan disini, pastikan fungsi Luncher tersebut ada jika anda memodifikasi atau mengganti luncher bawaan.
- **`entries.x`**: Registrasi daftar aplikasi untuk launcher menggunakan teknik *link-time optimization*.
- **`modules.h`**: *Centralized header inclusion* untuk mempermudah manajemen dependensi antar modul.

---

## Konfigurasi & Profil Performa

Efisiensi RunOX diatur melalui **`config.h`**. Pengguna dapat menyesuaikan profil daya berdasarkan kebutuhan spesifik:

| **Profil** | **Clock Speed** | **Loop Interval** | **Karakteristik** |
| --- | --- | --- | --- |
| **Standard** | 80 MHz | 100 ms | Keseimbangan daya dan responsivitas. |
| **Performance** | 240 MHz | 10 ms | Responsivitas maksimal  |
| **PowerSave** | 10 MHz | 200 ms | Konsumsi daya minimal untuk tugas pasif. |

> **Peringatan Teknis:** Penggunaan Clock Speed 10MHz sangat tidak disarankan untuk aplikasi dengan komputasi intensif atau komunikasi bus yang ketat (I2C/SPI) karena risiko ketidakstabilan *timing* sistem
> 

---

### Life Cycle & Memory Management

RunOX beroperasi dengan sistem **Clean Context Switching**. Ketika sebuah aplikasi dipanggil:

1. **Context Initialization**: Launcher melepaskan kontrol, dan aplikasi melakukan alokasi statis pada memori yang dibutuhkan.
2. **Execution**: Aplikasi memegang kendali penuh atas CPU loop.
3. **Graceful Exit**: Saat `Hold B` dideteksi, aplikasi wajib menghancurkan object lokal dan melepaskan pointer sebelum kendali dikembalikan ke Launcher.

> Hal ini memastikan bahwa penggunaan SRAM tetap flat dan tidak ada kebocoran memori (memory leak) meskipun sistem berjalan berhari-hari.
> 

## Skema Kontrol (User Interaction)

Input diproses melalui state manager M5Unified untuk memastikan respons yang presisi:

- **Click (A)**: Select / Konfirmasi / Trigger Aksi Utama.
- **Click (B | C)**: Navigasi (Next / Previous / Scroll).
- **Hold B (2s)**: **Force Exit** — Menghentikan loop aplikasi aktif dan kembali ke Launcher.
- **Hold C (6s)**: **Hard Shutdown** — Memutus daya sepenuhnya (`M5.Power.PowerOff()`).

---

## Pengembangan Aplikasi (Development Guide)

RunOX menggunakan arsitektur **Sequential Execution**. Agar sebuah aplikasi dapat berjalan secara harmonis di dalam ekosistem RunOX, pengembang wajib mengikuti format statis berikut.

### 1. Format Aplikasi Statis

Setiap aplikasi harus didefinisikan sebagai fungsi `void` tanpa argumen dan menggunakan *state variable* untuk mengelola inisialisasi.

```cpp
void app_nama_aplikasi() {
    // [INIT] Berjalan sekali saat aplikasi dimuat
    static bool initialized = false;
    static uint32_t lastUpdate = 0;

    if (!initialized) {
        d->fillScreen(BLACK);
        // Setup peripheral, font, atau state awal di sini
        initialized = true;
    }

    // [EXIT] Mekanisme keluar standar RunOX (Hold B 2 detik)
    if (M5.BtnB.pressedFor(2000)) {
        initialized = false; // Reset state untuk pemanggilan berikutnya
        load_app(app_launcher);
        return;
    }

    // [LOOP] Eksekusi logika berdasarkan interval yang ditentukan di config.h
    if (millis() - lastUpdate > SYS_LOOP_INTERVAL_MS || lastUpdate == 0) {
        lastUpdate = millis();

        d->startWrite();
        // Render UI dan Logic utama di sini
        d->endWrite();
    }
}
```

### 2. Cara Mendaftarkan  atau Melepas Aplikasi

RunOX menggunakan teknik **Link-time Optimization** melalui file `entries.x`. Ini memungkinkan modularitas tinggi tanpa perlu mengubah logika pada Base.

1. **Header**: Tambahkan deklarasi fungsi aplikasi Anda di file header yang sesuai atau di `modules.h`.
    
    ```cpp
    // module.h
    #include "nama_app.h"
    ```
    
2. **Registration**: Buka file `entries.x` dan daftarkan aplikasi Anda dengan format berikut:
    
    ```cpp
    // entries.x
    APP_ENTRY("Nama App1", nama_func_app_1)
    APP_ENTRY("Nama App2", nama_func_app_2)
    ```
    
3. **Compile**: Sistem akan secara otomatis menyertakan aplikasi tersebut ke dalam daftar navigasi pada Launcher.
4. **Melepas:** Untuk melepas aplikasi cukup dengan menghapus/mengomentari baris tersebut di file `entries.x`
    
    ```cpp
    // entries.x
    APP_ENTRY("Nama App1", nama_func_app_1)
    //APP_ENTRY("Nama App2", nama_func_app_2)
    ```
    

---

## Ekosistem Aplikasi Bawaan (Default Modules)

RunOX menyertakan sekumpulan aplikasi *core* yang dirancang dengan prinsip efisiensi tinggi:

### Clock (RTC BM8563)

Aplikasi penunjuk waktu presisi tinggi yang memanfaatkan chip RTC internal.

- **Fitur**: Tampilan waktu digital, pengaturan tanggal, dan sistem alarm.
- **Optimasi**: Menggunakan fitur *Hysteresis Wake* (Layar menyala otomatis saat pergelangan tangan diangkat) untuk menghemat daya.

> Di aplikasi ini tidak ada fitur untuk mengatur waktu, anda  perlu mengaturnya di dalama `void setup()` secara manual
> 

### Air Mouse (BLE HID + MPU6886)

Mengubah M5StickC Plus2 menjadi mouse nirkabel berbasis gerakan.

- **Fitur**: Kontrol kursor via sensor akselerometer dan giroskop.
- **Optimasi**: Berjalan pada *Standar mode* (80MHz) untuk konsumsi daya rendah.

### picotop (System Monitor)

Alat monitoring sumber daya perangkat secara *real-time*.

- **Metrik**: Penggunaan SRAM/PSRAM, Temperatur CPU, Frekuensi CPU, serta Voltase dan Persentase Baterai.
- **Tujuan**: Debugging dan manajemen kesehatan perangkat saat menjalankan aplikasi berat.

