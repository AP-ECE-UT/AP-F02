#include "image_editor.hpp"

#include <stdexcept>

#include "filter/filter.hpp"
#include "image/bmp.hpp"

ImageEditor::~ImageEditor() {
    for (Filter* f : filterPipe_) {
        delete f;
    }
}

void ImageEditor::addFilter(Filter* filter) {
    filterPipe_.push_back(filter);
    viewPipe_.push_back({});
}

void ImageEditor::addFilter(Filter* filter, ViewInfo view) {
    filterPipe_.push_back(filter);
    viewPipe_.push_back(view);
}

void ImageEditor::editImage(const std::string& filename, const std::string& output) {
    Bmp img;
    Image::ReadResult res = img.read(filename);
    if (res != Image::ReadResult::success) {
        throw std::runtime_error("Failed to read file.");
    }

    for (unsigned i = 0; i < filterPipe_.size(); ++i) {
        ImageView imagePart(&img);
        if (viewPipe_[i].width != 0) {
            imagePart.setView(viewPipe_[i]);
        }
        filterPipe_[i]->apply(&imagePart);
    }
    img.write(output);
}
