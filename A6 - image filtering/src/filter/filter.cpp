#include "filter.hpp"

void Filter::setImage(Image* img) {
    img_ = img;
}

void Filter::apply() {
    apply(img_);
}
