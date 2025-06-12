/**********************************************************************************
 * Copyright (C) 2025 Craig Setera
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 **********************************************************************************/
#include "QEMU_NeoMatrix.h"

void QEMU_NeoMatrix::begin(int32_t speed) {
    gfx.begin();
}


void QEMU_NeoMatrix::startWrite() {
    gfx.startWrite();
}

void QEMU_NeoMatrix::endWrite() {
    gfx.endWrite();
}

void QEMU_NeoMatrix::clear() {
    fillScreen(0);
}

void QEMU_NeoMatrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (inBounds(x, y)) {
        int16_t x0 = x * (led_pixel_size + gutter_pixel_size);
        int16_t y0 = y * (led_pixel_size + gutter_pixel_size);

        gfx.fillRect(x0, y0, led_pixel_size, led_pixel_size, color);
    }
}

void QEMU_NeoMatrix::fillScreen(uint16_t color) {
    gfx.fillScreen(color);
}

bool QEMU_NeoMatrix::inBounds(int16_t x, int16_t y) {
    return (x >= 0) && (x < matrix_width) && (y >= 0) && (y < matrix_height);
}

void QEMU_NeoMatrix::show() {
}
