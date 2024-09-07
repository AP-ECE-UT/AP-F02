#include "color_manip.hpp"

#include "../../image/image.hpp"

void ColorManip::apply(Image* img) {
    for (int row = 0; row < img->height(); ++row) {
        for (int col = 0; col < img->width(); ++col) {
            auto& pixel = (*img)(row, col);
            pixel = change(pixel);
        }
    }
}
