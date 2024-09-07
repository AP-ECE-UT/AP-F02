#include "grayscale.hpp"

Pixel GrayScale::change(Pixel p) const {
    auto gray = (p.red + p.grn + p.blu) / 3;
    return Pixel(gray, gray, gray);
}
