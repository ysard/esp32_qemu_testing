/**********************************************************************************
 * Copyright (C) 2025 Craig Setera
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 **********************************************************************************/
#pragma once

#include "Arduino_GFX.h"
#include "esp_lcd_qemu_rgb.h"

/**
 * @brief Wraps the Arduino_GFX library around the ESP32 QEMU RGB 
 * Framebuffer.
 */
class QEMU_GFX : public Arduino_GFX {
public:
    QEMU_GFX(int16_t w, int16_t h): Arduino_GFX(w, h),
        _height(h),
        _width(w),
        frame_buffer(nullptr),
        panel_handle(nullptr),
        txnCounter(0) {}

    virtual bool begin(int32_t speed = GFX_NOT_DEFINED);
    virtual void writePixelPreclipped(int16_t x, int16_t y, uint16_t color);

    virtual void startWrite();
    virtual void endWrite(void);

private:
    int16_t _width;
    int16_t _height;

    esp_lcd_panel_handle_t panel_handle;
    void* frame_buffer;
    int txnCounter;
};
