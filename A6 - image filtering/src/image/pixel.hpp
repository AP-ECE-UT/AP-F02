#ifndef PIXEL_HPP_INCLUDE
#define PIXEL_HPP_INCLUDE

#include <cstdint>

struct Pixel {
    Pixel() = default;
    Pixel(uint8_t r, uint8_t g, uint8_t b)
        : blu(b), grn(g), red(r) {}
    uint8_t blu;
    uint8_t grn;
    uint8_t red;
};

#endif // PIXEL_HPP_INCLUDE
