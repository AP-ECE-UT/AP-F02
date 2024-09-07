#ifndef COLOR_MANIP_HPP_INCLUDE
#define COLOR_MANIP_HPP_INCLUDE

#include "../../image/pixel.hpp"
#include "../filter.hpp"

class ColorManip : public Filter {
public:
    using Filter::Filter;

    void apply(Image* img) override;

protected:
    virtual Pixel change(Pixel p) const = 0;
};

#endif // COLOR_MANIP_HPP_INCLUDE
