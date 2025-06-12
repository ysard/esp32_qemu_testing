/**********************************************************************************
 * Copyright (C) 2025 Craig Setera
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 **********************************************************************************/
#include "QEMU_GFX.h"


bool QEMU_GFX::begin(int32_t speed) {
    esp_lcd_rgb_qemu_config_t panel_config = {
        .width = (uint32_t) _width,
        .height = (uint32_t) _height,
        .bpp = RGB_QEMU_BPP_16
    };

    ESP_ERROR_CHECK(esp_lcd_new_rgb_qemu(&panel_config, &panel_handle));

    return true;
}

void QEMU_GFX::startWrite() {
    if (txnCounter == 0) {
        esp_lcd_rgb_qemu_get_frame_buffer(panel_handle, &frame_buffer);
    }

    txnCounter++;
}

void QEMU_GFX::endWrite(void) {
    txnCounter--;

    if (txnCounter == 0) {
        esp_lcd_rgb_qemu_refresh(panel_handle);
        frame_buffer = nullptr;
    }
}

void QEMU_GFX::writePixelPreclipped(int16_t x, int16_t y, uint16_t color) {
    if (frame_buffer != NULL) {
        uint16_t* pixel_data = (uint16_t*)frame_buffer;

        auto offset = (y * _width) + x;
        pixel_data[offset] = color;
    }
}
