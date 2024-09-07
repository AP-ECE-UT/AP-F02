#ifndef INVERT_HPP_INCLUDE
#define INVERT_HPP_INCLUDE

#include "color_manip.hpp"

class Invert : public ColorManip {
public:
    using ColorManip::ColorManip;

private:
    Pixel change(Pixel p) const override;
};

#endif // INVERT_HPP_INCLUDE
