#include <Arduino.h>
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <qemu_matrix/QEMU_NeoMatrix.h>

#define LED_PIXEL_SIZE 20
#define GUTTER_PIXEL_SIZE 4
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 16

#define WIDTH 800
#define HEIGHT 400

QEMU_NeoMatrix matrix(MATRIX_WIDTH, MATRIX_HEIGHT, LED_PIXEL_SIZE, GUTTER_PIXEL_SIZE);

void setup() {
    #ifdef ESP32_QEMU
        // Give QEMU cache time to stabilize
        delay(100);
    #endif

    Serial.begin(115200);

    #ifdef ESP32_QEMU
        // Disable watchdogs that might interfere
        esp_task_wdt_deinit();
        
        // Explicit yield to ensure cache coherency
        yield();
    #endif
    
    matrix.begin();
}

void loop() {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            matrix.startWrite();
            matrix.clear();
            matrix.drawPixel(x, y, RGB565_RED);
            matrix.endWrite();

            delay(100);
        }
    }
}