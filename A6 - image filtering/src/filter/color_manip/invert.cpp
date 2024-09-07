#include "invert.hpp"

Pixel Invert::change(Pixel p) const {
    return Pixel(255 - p.red, 255 - p.grn, 255 - p.blu);
}
