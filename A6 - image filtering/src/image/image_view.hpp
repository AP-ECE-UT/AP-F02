#ifndef IMAGE_VIEW_HPP_INCLUDE
#define IMAGE_VIEW_HPP_INCLUDE

#include "image.hpp"

struct ViewInfo {
    int row;
    int col;
    int width;
    int height;
};

class ImageView : public Image {
public:
    ImageView(Image* img);
    ImageView(Image* img, ViewInfo info);

    void setView(ViewInfo info);

    int width() const override;
    int height() const override;

    Pixel operator()(int row, int col) const override;
    Pixel& operator()(int row, int col) override;

private:
    Image* img_ = nullptr;
    ViewInfo info_;
};

#endif // IMAGE_VIEW_HPP_INCLUDE
