# RunOX-M5Stick Plus 2

**RunOX** adalah *firmware* untuk M5StickC Plus2 yang dibangun menggunakan pola **Modular App Framework**. Konsep arsitektur ini memisahkan kodenya menjadi tiga komponen utama:

- **Core System:** Mengani interaksi hardware dasar dan inisialisasi awal.
- **Registry Layer:** Mengelola tabel pendaftaran (*entry*) modul aplikasi secara bersih tanpa perlu memodifikasi *core logic*.
- **App Modules:** Tempat penyimpanan logika aplikasi terisolasi yang bisa dipasang atau dilepas dengan rapi.

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
make compile
```

**D. Unggah (Upload)**
Ganti `PORT` dengan alamat port perangkat Anda (misal: `/dev/ttyUSB0` atau `/dev/ttyACM0` atau `COM3`):

```bash
make upload
```

## Arsitektur Sistem

RunOX membagi tanggung jawab sistem ke dalam tiga lapisan:

**1. The Base (Hardware Abstraction Layer)**

Dibangun di atas **Arduino ESP32 Framework** dan **M5Unified**. Base berfungsi sebagai fondasi statis yang menangani komunikasi *low-level* dengan periferal. Base bersifat *immutable* bagi lapisan aplikasi untuk menjaga integritas sistem.

**2. The Launcher (System Gateway)**

Aplikasi manajer yang berfungsi sebagai *entry point*. Launcher dipanggil secara otomatis oleh Base saat *cold boot* atau ketika sebuah aplikasi melepaskan kendali (*exit*).

**3. The Application (User Space)**

Lapisan tertinggi dengan kontrol penuh. Developer memiliki akses mutlak ke siklus CPU dan perangkat keras. **Catatan:** Manajemen sumber daya (SRAM, PSRAM, daya, dan periferal) adalah tanggung jawab penuh pengembang aplikasi.

> **Catatan:** Tanggung jawab manajemen sumber daya (memori, periferal, daya) sepenuhnya dilepas kepada pembuat lapisan Aplikasi.
> 

---

### Struktur File & Arsitektur

RunOX menggunakan pola arsitektur modular untuk menjaga pemisahan kode (*decoupling*) antara sistem utama, tabel pendaftaran aplikasi, dan modul-modul fitur.

- **RunOX-M5Stack.ino**: Executive Entry Point. Menangani inisialisasi awal hardware (`M5.begin()`) dan mengeksekusi *main loop*.
- **system.h / system.cpp**: Interface jembatan dan implementasi API sistem utama. Mengatur logika core, state global, serta deklarasi Launcher. Jika Anda mengganti Launcher bawaan, pastikan fungsi Launcher tersebut dideklarasikan di sini.
- **config.h**: *Single source of truth* untuk definisi konstanta global (Clock speed, intervals, pinout, dll).
- **entries.x**: Tabel registrasi daftar modul aplikasi yang terhubung ke Launcher menggunakan teknik Macro X-Macro / table-driven registry.
- **modules.h**: Centralized header inclusion untuk mengimpor seluruh header modul aplikasi dari folder `src/`.
- **src/**: Folder direktori utama yang menampung seluruh implementasi modul aplikasi (dulu `app_*`) dan komponen launcher secara terisolasi.

---

## Konfigurasi & Profil Performa

Efisiensi RunOX diatur melalui **`config.h`**. Pengguna dapat menyesuaikan profil daya berdasarkan kebutuhan spesifik:

| **Profil** | **Clock Speed** | **Loop Interval** | **Karakteristik** |
| --- | --- | --- | --- |
| **Standard** | 80 MHz | 100 ms | Keseimbangan daya dan responsivitas. |
| **Performance** | 240 MHz | 10 ms | Responsivitas maksimal |
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
#include "../modules.h"

void app_contoh() {
    // 1. [INIT]
    bool isRunning = true;
    
    // 2. [EVENT LOOP]
    while (isRunning) {
        M5.update();

        // [EXIT] 
        if (M5.BtnB.pressedFor(BTN_EXIT_TIMEOUT_MS)) {
            break; 
        }

        // [LOGIC & UI] 
        // ...

        // [TASK YIELD] 
        vTaskDelay(pdMS_TO_TICKS(20)); 
    }

    // 3. [CLEANUP]
}
```

### 2. Cara Mendaftarkan atau Melepas Aplikasi

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
