#include "kernel.hpp"

#include "../../image/image.hpp"

Kernel::Kernel(Matrix km, bool normalize)
    : Filter(),
      km_(normalize ? normalizeMatrix(km) : km) {}

void Kernel::apply(Image* img) {
    static const int dirX[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
    static const int dirY[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

    const Image imgCpy(*img);

    auto inImage = [imgCpy](int row, int col) -> bool {
        return row >= 0 && row < imgCpy.height() && col >= 0 && col < imgCpy.width();
    };

    for (int row = 0; row < img->height(); ++row) {
        for (int col = 0; col < img->width(); ++col) {
            float red = 0, green = 0, blue = 0;
            for (int i = 0; i < 9; ++i) {
                Pixel pixel = imgCpy(row, col);
                if (inImage(row + dirX[i], col + dirY[i])) {
                    pixel = imgCpy(row + dirX[i], col + dirY[i]);
                }
                red += km_[i] * pixel.red;
                green += km_[i] * pixel.grn;
                blue += km_[i] * pixel.blu;
            }

            auto& pixel = (*img)(row, col);
            pixel.red = std::min(255, std::max<int>(0, red));
            pixel.grn = std::min(255, std::max<int>(0, green));
            pixel.blu = std::min(255, std::max<int>(0, blue));
        }
    }
}

Kernel::Matrix Kernel::normalizeMatrix(Kernel::Matrix km) {
    constexpr float epsilon = 1e-3;
    float sum = 0;
    for (int i = 0; i < 9; ++i) {
        sum += km[i];
    }
    if (sum >= -epsilon && sum <= epsilon) return km;
    for (int i = 0; i < 9; ++i) {
        km[i] /= sum;
    }
    return km;
}
