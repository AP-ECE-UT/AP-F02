#include "image_view.hpp"

ImageView::ImageView(Image* img)
    : img_(img),
      info_{0, 0, img->width(), img->height()} {}

ImageView::ImageView(Image* img, ViewInfo info)
    : img_(img),
      info_(info) {}

void ImageView::setView(ViewInfo info) {
    info_ = info;
}

int ImageView::width() const { return info_.width; }
int ImageView::height() const { return info_.height; }

Pixel ImageView::operator()(int row, int col) const {
    return (*img_)(info_.row + row, info_.col + col);
}

Pixel& ImageView::operator()(int row, int col) {
    return (*img_)(info_.row + row, info_.col + col);
}
