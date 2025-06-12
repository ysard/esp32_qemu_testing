/**********************************************************************************
 * Copyright (C) 2025 Craig Setera
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 **********************************************************************************/
#pragma once

#include <Adafruit_NeoMatrix.h>
#include <stdint.h>
#include <qemu_matrix/QEMU_GFX.h>

/*
#define LED_PIXEL_SIZE 10
#define GUTTER_PIXEL_SIZE 2
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 16
*/

#define TOTAL_PIXELS(led_count, led_size, gutter_size) \
    ((led_count) * (led_size) + ((led_count) - 1) * (gutter_size))

/**
 * @brief Takes advantage of the ESP32 QEMU RGB video output to act like a Neomatrix.
 */
class QEMU_NeoMatrix : public Adafruit_NeoMatrix {
public:
    QEMU_NeoMatrix(int matrix_width, int matrix_height, int led_pixel_size, int gutter_pixel_size) :
        Adafruit_NeoMatrix(matrix_width, matrix_height),
        matrix_width(matrix_width),
        matrix_height(matrix_height),
        led_pixel_size(led_pixel_size),
        gutter_pixel_size(gutter_pixel_size),
        gfx(QEMU_GFX(TOTAL_PIXELS(matrix_width, led_pixel_size, gutter_pixel_size), TOTAL_PIXELS(matrix_height, led_pixel_size, gutter_pixel_size)))
        {

        }

    virtual void    begin(int32_t speed = -1);

    virtual void    startWrite();
    virtual void    endWrite();

    virtual void    clear();
    virtual void    drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void    fillScreen(uint16_t color);
    virtual void    show();

private:
    int         matrix_width;
    int         matrix_height;
    int         led_pixel_size;
    int         gutter_pixel_size;
    QEMU_GFX    gfx;

    bool        inBounds(int16_t x, int16_t y);
};
