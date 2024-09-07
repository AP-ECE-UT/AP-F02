#ifndef GRAYSCALE_HPP_INCLUDE
#define GRAYSCALE_HPP_INCLUDE

#include "color_manip.hpp"

class GrayScale : public ColorManip {
public:
    using ColorManip::ColorManip;

private:
    Pixel change(Pixel p) const override;
};

#endif // GRAYSCALE_HPP_INCLUDE
